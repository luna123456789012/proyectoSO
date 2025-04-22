DROP DATABASE IF EXISTS T2_juego;
CREATE DATABASE T2_juego;
USE T2_juego;

CREATE TABLE jugadores (
    ID_j INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
    usuario TEXT NOT NULL,
    contraseña TEXT
) ENGINE = InnoDB;

CREATE TABLE partida (
    ID_p INTEGER PRIMARY KEY AUTO_INCREMENT,
    tipo TEXT NOT NULL,
    num_jugadores INTEGER NOT NULL,
    ID_ganador INTEGER,
    FOREIGN KEY (ID_ganador) REFERENCES jugadores(ID_j)
) ENGINE = InnoDB;


CREATE TABLE jugadores_partida (
    ID_j INT NOT NULL,
    ID_p INT NOT NULL,
    PRIMARY KEY (ID_j, ID_p),
    FOREIGN KEY (ID_j) REFERENCES jugadores(ID_j),
    FOREIGN KEY (ID_p) REFERENCES partida(ID_p)
) ENGINE = InnoDB;

CREATE TABLE cartas(
	ID_c INT NOT NULL,
	color TEXT,
	numero INT
) ENGINE = InnoDB;

INSERT INTO jugadores (ID_j, usuario, contraseña) VALUES (1, 'luna', 'luna');
INSERT INTO jugadores (ID_j, usuario, contraseña) VALUES (2, 'test', 'test');
INSERT INTO jugadores (ID_j, usuario, contraseña) VALUES (3, '1', '1');
INSERT INTO jugadores (ID_j, usuario, contraseña) VALUES (4, '2', '2');

INSERT INTO cartas (ID_c, color, numero) VALUES
	(1, 'rojo', 0),
	(2, 'rojo', 1),
	(3, 'rojo', 2),
	(4, 'rojo', 3),
	(5, 'rojo', 4),
	(6, 'rojo', 5),
	(7, 'rojo', 6),
	(8, 'rojo', 7),
	(9, 'rojo', 8),
	(10, 'rojo', 9),

	(11, 'amarillo', 0),
	(12, 'amarillo', 1),
	(13, 'amarillo', 2),
	(14, 'amarillo', 3),
	(15, 'amarillo', 4),
	(16, 'amarillo', 5),
	(17, 'amarillo', 6),
	(18, 'amarillo', 7),
	(19, 'amarillo', 8),
	(20, 'amarillo', 9),
	
	
	(21, 'verde', 0),
	(22, 'verde', 1),
	(23, 'verde', 2),
	(24, 'verde', 3),
	(25, 'verde', 4),
	(26, 'verde', 5),
	(27, 'verde', 6),
	(28, 'verde', 7),
	(29, 'verde', 8),
	(30, 'verde', 9),
	
	(31, 'azul', 0),
	(32, 'azul', 1),
	(33, 'azul', 2),
	(34, 'azul', 3),
	(35, 'azul', 4),
	(36, 'azul', 5),
	(37, 'azul', 6),
	(38, 'azul', 7),
	(39, 'azul', 8),
	(40, 'azul', 9);
	


