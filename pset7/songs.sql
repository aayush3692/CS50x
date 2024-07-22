--In 1.sql, write a SQL query to list the names of all songs in the database.
--Your query should output a table with a single column for the name of each song.
SELECT "name" from songs;

--In 2.sql, write a SQL query to list the names of all songs in increasing order of tempo.
--Your query should output a table with a single column for the name of each song.
select "name" from songs order by "tempo" asc;

--In 3.sql, write a SQL query to list the names of the top 5 longest songs, in descending order of length.
--Your query should output a table with a single column for the name of each song.
select "name" from songs order by "duration_ms" desc limit 5;

--In 4.sql, write a SQL query that lists the names of any songs that have danceability, energy, and valence greater than 0.75.
--Your query should output a table with a single column for the name of each song.

SELECT "name" from songs where "danceability" > 0.75 AND "energy" > 0.75 AND "valence" > 0.75;

--In 5.sql, write a SQL query that returns the average energy of all the songs.
--Your query should output a table with a single column and a single row containing the average energy.
SELECT avg("energy") from songs;

--In 6.sql, write a SQL query that lists the names of songs that are by Post Malone.
--Your query should output a table with a single column for the name of each song.
--You should not make any assumptions about what Post Malone’s artist_id is.

Select "id" from "artists" where name="Post Malone";

select "name" from songs WHERE "artist_id"=54;

--In 7.sql, write a SQL query that returns the average energy of songs that are by Drake.
--Your query should output a table with a single column and a single row containing the average energy.
--You should not make any assumptions about what Drake’s artist_id is.

select "id" from "artists" where "name" = "Drake";

select AVG("energy") from songs where artist_id=23;

--In 8.sql, write a SQL query that lists the names of the songs that feature other artists.
--Songs that feature other artists will include “feat.” in the name of the song.
--Your query should output a table with a single column for the name of each song.

select "name" from songs where "name" like "%feat%";



