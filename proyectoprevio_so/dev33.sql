DROP DATABASE IF EXISTS juego;
CREATE DATABASE juego;
USE juego;
CREATE TABLE jugadores (
	ID_j INT PRIMARY KEY NOT NULL,
	usuario TEXT NOT NULL,
	contraseña TEXT,
)ENGINE = InnoDB;

INSERT INTO jugadores (ID_j, usuario, contraseña) VALUES (1, 'alvaro', 'piano42');
INSERT INTO jugadores (ID_j, usuario, contraseña) VALUES (2, 'marta', '5pistacho');
INSERT INTO jugadores (ID_j, usuario, contraseña) VALUES (3, 'paula', '86flor');
INSERT INTO jugadores (ID_j, usuario, contraseña) VALUES (4, 'didac', 'casa00');
INSERT INTO jugadores (ID_j, usuario, contraseña) VALUES (5, 'daniel', 'so2025');