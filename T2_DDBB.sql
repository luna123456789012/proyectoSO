DROP DATABASE IF EXISTS T2_juego;
CREATE DATABASE T2_juego;
USE T2_juego;

CREATE TABLE jugadores (
    ID_j INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
    usuario TEXT NOT NULL,
    password TEXT
) ENGINE = InnoDB;

CREATE TABLE partida (
    ID_partida INTEGER PRIMARY KEY AUTO_INCREMENT,
    tipo TEXT,
    num_jugadores INTEGER NOT NULL,
    ID_ganador INTEGER NOT NULL,
    hora_inicio DATETIME,
    hora_fin DATETIME,
    FOREIGN KEY (ID_ganador) REFERENCES jugadores(ID_j) ON DELETE CASCADE
) ENGINE = InnoDB;


CREATE TABLE jugadores_partida (
    ID_j INT NOT NULL,
    ID_p INT NOT NULL,
    PRIMARY KEY (ID_j, ID_p),
    FOREIGN KEY (ID_j) REFERENCES jugadores(ID_j) ON DELETE CASCADE,
    FOREIGN KEY (ID_p) REFERENCES partida(ID_partida) ON DELETE CASCADE
) ENGINE = InnoDB;

CREATE TABLE cartas(ID_c INT NOT NULL,
     color TEXT,
     numero INT,
     total INT,
     sobran INT
) ENGINE = InnoDB;

INSERT INTO cartas (ID_c, color, numero, total, sobran) VALUES
	(1, 'rojo', 0, 3, 3),
	(2, 'rojo', 1, 3, 3),
	(3, 'rojo', 2, 3, 3),
	(4, 'rojo', 3, 3, 3),
	(5, 'rojo', 4, 3, 3),
	(6, 'rojo', 5, 3, 3),
	(7, 'rojo', 6, 3, 3),
	(8, 'rojo', 7, 3, 3),
	(9, 'rojo', 8, 3, 3),
	(10, 'rojo', 9, 3, 3),

        (11, 'amarillo', 0, 3, 3),
	(12, 'amarillo', 1, 3, 3),
	(13, 'amarillo', 2, 3, 3),
	(14, 'amarillo', 3, 3, 3),
	(15, 'amarillo', 4, 3, 3),
	(16, 'amarillo', 5, 3, 3),
	(17, 'amarillo', 6, 3, 3),
	(18, 'amarillo', 7, 3, 3),
	(19, 'amarillo', 8, 3, 3),
	(20, 'amarillo', 9, 3, 3),

        (21, 'verde', 0, 3, 3),
	(22, 'verde', 1, 3, 3),
	(23, 'verde', 2, 3, 3),
	(24, 'verde', 3, 3, 3),
	(25, 'verde', 4, 3, 3),
	(26, 'verde', 5, 3, 3),
	(27, 'verde', 6, 3, 3),
	(28, 'verde', 7, 3, 3),
	(29, 'verde', 8, 3, 3),
	(30, 'verde', 9, 3, 3),
	
	(31, 'azul', 0, 3, 3),
	(32, 'azul', 1, 3, 3),
	(33, 'azul', 2, 3, 3),
	(34, 'azul', 3, 3, 3),
	(35, 'azul', 4, 3, 3),
	(36, 'azul', 5, 3, 3),
	(37, 'azul', 6, 3, 3),
	(38, 'azul', 7, 3, 3),
	(39, 'azul', 8, 3, 3),
	(40, 'azul', 9, 3, 3);

INSERT INTO jugadores (ID_j, usuario, password) VALUES
(1, '1', '1'),
(2, '2', '2'),
(3, 'test', 'test'),
(4, 'luna', 'luna');

INSERT INTO partida (ID_partida, tipo, num_jugadores, ID_ganador, hora_inicio, hora_fin) VALUES
(1, '', 2, 1, '2025-06-10 20:00:00', '2025-06-10 20:30:00'),
(2, '', 3, 4, '2025-06-11 18:00:00', '2025-06-11 18:45:00'),
(3, '', 2, 2, '2025-06-12 21:00:00', '2025-06-12 21:20:00');

INSERT INTO jugadores_partida (ID_j, ID_p) VALUES
(1, 1),
(2, 1),
(1, 2),
(4, 2),
(3, 2),
(2, 3),
(3, 3);
	