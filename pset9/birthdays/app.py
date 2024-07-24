from functools import total_ordering
import os
from pprint import pprint
import profile

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd
from datetime import datetime




# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True
app.config['flask_profiler'] = {
    "enable": app.config['TEMPLATES_AUTO_RELOAD'],
    "storage": {
        "engine": "sqlite"
    },
    "basicAuth": {
        "enabled": True,
        "username": "admin",
        "password": "admin"
    }
}

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    os.environ["API_KEY"] = "pk_bf4b7e24d44e4d15a4db9e8d817efdb5"


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
        # stocks in hand
    # getting information from the wallet
    in_wallet = db.execute("SELECT stock_name, stock_symbol, size FROM wallet WHERE user_id = ? AND size > ?", session["user_id"], 0)
    # cash in hand
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    cash = round(cash, 2)

    # total value of each stock
    stocks_value = []
    # total value of portfolio
    total_value = 0

    # getting the value of each stock price * size
    for i in in_wallet:
        symbol = i["stock_symbol"]
        size = db.execute("SELECT size FROM wallet WHERE user_id = ? AND stock_symbol = ?", session["user_id"], symbol)[0]["size"]
        price = lookup(symbol)["price"]

        # total value of that stock is
        value = size * price
        value = round(value, 2)
        total_value = total_value + value
        stock_value_dic = {"price":[],"value":[]}
        stock_value_dic["value"].append(value)
        stock_value_dic["price"].append(price)
        stocks_value.append(stock_value_dic)

    cash = round(cash, 2)
    total_value = total_value + cash
    total_value = round(total_value, 2)


    if request.method == "GET":
        return render_template("/index.html", in_wallet = in_wallet, total_value = total_value, cash = cash, stocks_value = stocks_value)
    if request.method == "POST":
        return render_template("/index.html", in_wallet = in_wallet, total_value = total_value, cash = cash, stocks_value = stocks_value)



@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("/buy.html")
    if request.method == "POST":
        if not request.form.get("name"):
            return apology("Please choose a stock")
        if not request.form.get("shares"):
            return apology("please choose the amount of shares")
        if int(request.form.get("shares")) < 1:
            return apology("please choose a postive number")

    # fetching stock details
    if (lookup(request.form.get("name").upper())) is None:
        return apology("wrong symbol")
    price = lookup(request.form.get("name").upper())["price"]
    stock_name = lookup(request.form.get("name").upper())["name"]
    stock_symbol = lookup(request.form.get("name").upper())["symbol"]

    # how much cash the user has
    cash_in_hand = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    # checking if there is enough money
    if price * int(request.form.get("shares")) > cash_in_hand:
        return apology("insufficient funds")
    # UPDATING WITH MONEY LEFT IN HAND
    shares = int(request.form.get("shares"))
    cash_left = cash_in_hand - price * shares
    db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_left, session["user_id"])
    # RECORDING TRANSACTION IN HISTORY
    now = datetime.now()
    db.execute("INSERT INTO history (date, status, size, stock_name, stock_symbol, user_id, price) VALUES (?,?,?,?,?,?,?)",now, "Buy", shares, stock_name, stock_symbol, session["user_id"], price)
    # UPDATING THE WALLET
        # inserting shares name in wallet
    symbol_in_wallet = db.execute("SELECT stock_symbol FROM wallet WHERE user_id = ? AND stock_symbol = ?", session["user_id"], stock_symbol)
    if len(symbol_in_wallet) == 0:
        #insert the symbol of the stock
        db.execute("INSERT INTO wallet (stock_symbol, stock_name, user_id) VALUES (?, ?, ?)",stock_symbol, stock_name, session["user_id"])
        # Updating amount of shares (size)
    size_inhand = db.execute("SELECT size FROM wallet WHERE stock_symbol = ? AND user_id = ?", stock_symbol, session["user_id"])
    size_after_buy = size_inhand =+ shares
    db.execute("UPDATE wallet SET size = ? WHERE stock_symbol = ? AND user_id = ?", size_after_buy, stock_symbol, session["user_id"])

    flash("Successfully Bough!")

    return render_template("/buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    trans_details = db.execute("SELECT stock_name, stock_symbol, size, date, status, price FROM history WHERE user_id = ? ORDER BY date DESC", session["user_id"])
    return render_template("/history.html", transactions = trans_details)




@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("/quote.html")
    # validating symbol entry
    if not request.form.get("buy"):
        return apoloy("must provide a symbol")
    # converting the symbol to upper
    stock = lookup(request.form.get("buy").upper())
    # checking the return of lookup
    if stock == None:
        return apology("no stock with that symbol")
    return render_template("/quoted.html", stock_name = stock["name"], stock_price = stock["price"], stock_symbol = stock["symbol"]  )



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # validating username
        if not request.form.get("username"):
            return apology("must provide username", 403)
        # validating password
        elif not request.form.get("password"):
            return apology("must provide password", 403)
        elif not request.form.get("confirm"):
            return apology("must confirm password")
        # confirming the password and validating it
        if request.form.get("password") != request.form.get("confirm"):
            return apology("Password and Confirmation dont match")

        # checking database if the username already exists
        if db.execute("SELECT username FROM users WHERE username = ?", request.form.get("username")):
            return apology("username not available")

        else:
            # hashing the pass
            hashed_pass = generate_password_hash(request.form.get("password"))
            # inserting the account credentials in database
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get("username"), hashed_pass)
        return redirect("/login")

    else:
        # if user came from GET
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # checking for valid input

    # stocks he has in wallet in html selector
    names = db.execute("SELECT stock_name FROM wallet WHERE size > 0 AND user_id = ?", session["user_id"])
    names_listed=[]
    for row in names:
        next = row["stock_name"]
        names_listed.append(next)

    if request.method == "GET":
        return render_template("./sell.html", listed_names = names_listed)
    if request.method == "POST":
        amount = request.form.get("amount")
        if not amount.isdigit():
            return apology("please enter a valid amount")

        # stocks he has in the wallet
        name = request.form.get("symbol")

        # if the input is not in the wallet return apology
        if name not in names_listed:
            return apology("You dont any of that stock")
        if not name:
            return apology("Please choose a stock")

        # confirming he is not selling something he doesnt have
        amount_in_wallet = db.execute("SELECT size FROM wallet WHERE stock_name = ? AND user_id = ?", name, session["user_id"])[0]["size"]

        if int(amount) > int(amount_in_wallet):
            return apology("You dont have this amount of shares")

        # update database for the new amount
        upd_amount = int(amount_in_wallet) - int(amount)
        db.execute("UPDATE wallet SET size = ? WHERE stock_name = ? AND user_id = ?", upd_amount, name, session["user_id"])

        # getting the stock price
        name = name
        symbol = db.execute("SELECT stock_symbol FROM wallet WHERE stock_name = ?", name)
        symbol = symbol[0]["stock_symbol"]
        price = lookup(symbol)["price"]

        # amount of cash in the wallet
        cash_in_hand = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        # updating the cash in wallet
        cash_updt = cash_in_hand + (float(amount) * float(price))
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_updt, session["user_id"])

        # updating the history table
        stocks_in_wallet = db.execute("SELECT stock_symbol FROM wallet WHERE stock_name = ?", name)[0]["stock_symbol"]
        now = datetime.now()
        db.execute("INSERT INTO history (date, status, size, stock_name, stock_symbol, user_id, price) VALUES (?,?,?,?,?,?,?)",now, "Sell", amount, name, stocks_in_wallet, session["user_id"], price)
        flash("Successfully Sold!")
        return render_template("./sell.html", listed_names = names_listed)
