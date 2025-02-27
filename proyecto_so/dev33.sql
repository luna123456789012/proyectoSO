DROP DATABASE IF EXISTS juego;
CREATE DATABASE juego;
USE juego;
CREATE TABLE jugadores (
	ID_j INT PRIMARY KEY NOT NULL,
	usuario TEXT NOT NULL
)ENGINE = InnoDB;

INSERT INTO jugadores (ID_j, usuario) VALUES (1, 'alvaro');
INSERT INTO jugadores (ID_j, usuario) VALUES (2, 'marta');
INSERT INTO jugadores (ID_j, usuario) VALUES (3, 'paula');
INSERT INTO jugadores (ID_j, usuario) VALUES (4, 'didac');
INSERT INTO jugadores (ID_j, usuario) VALUES (5, 'daniel');