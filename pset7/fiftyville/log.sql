-- Keep a log of any SQL queries you execute as you solve the mystery.

select description from crime_scene_reports where year = 2023 AND month = 7 and day = 28;

select transcript from interviews where year = 2023 AND month = 7 and day = 28 AND transcript like "%bakery%";

select bakery_security_logs.activity,
bakery_security_logs.license_plate, people.name FROM people
 JOIN
bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
WHERE bakery_security_logs.year = 2023
AND bakery_security_logs.month = 7
AND bakery_security_logs.day =28
AND bakery_security_logs.hour = 10
AND bakery_security_logs.minute >= 15
AND bakery_security_logs.minute <= 25;

select people.name, atm_transactions.transaction_type from people
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
where atm_transactions.year = 2023
AND atm_transactions.month = 7
AND atm_transactions.day = 28
AND atm_location = "Leggett Street"
AND atm_transactions.transaction_type = "withdraw";



update phone_calls set caller_name = people.name
from people where phone_calls.caller = people.phone_number;

update phone_calls set receiver_name = people.name from people
where phone_calls.receiver = people.phone_number;

SELECT caller,caller_name, receiver, receiver_name FROM phone_calls
WHERE year = 2023 AND month = 7 and day = 28 and duration < 60;

--update flights set origin_airport_id = airports.city
--from airports where flights.origin_airport_id = airports.id;

--update flights set destination_airport_id = airports.city
--from airports where flights.destination_airport_id = airports.id;

--Select id, hour,minute, origin_airport_id, destination_airport_id  from flights
--
--where year = 2023 AND month = 7 AND day = 29 order by hour ASC limit 1;

select name from people join passengers ON
passengers.passport_number = people.passport_number
where passengers.flight_id = (
    select id from flights where year = 2023 AND month = 7 AND day = 29 AND
    origin_airport_id=(
        SELECT id from airports where city = "Fiftyville"
    )order by hour, minute limit 1);

select name from people join passengers ON
passengers.passport_number = people.passport_number
JOIN flights ON flights.id = passengers.flight_id where
(flights.year = 2023 AND flights.month = 7 AND flights.day = 29
and flights.id = 36)
AND name IN (select phone_calls.caller_name FRom phone_calls
WHERE year = 2023
AND month = 7
and day = 28
and duration < 60)
AND name IN (select people.name from people
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
where atm_transactions.year = 2023
AND atm_transactions.month = 7
AND atm_transactions.day = 28
AND atm_location = "Leggett Street"
AND atm_transactions.transaction_type = "withdraw")
AND name IN
(select people.name from people
JOIN bakery_security_logs ON
bakery_security_logs.license_plate = people.license_plate
WHERE bakery_security_logs.year = 2023
AND bakery_security_logs.month = 7
AND bakery_security_logs.day =28
AND bakery_security_logs.hour = 10
AND bakery_security_logs.minute >= 15
AND bakery_security_logs.minute <= 25);

