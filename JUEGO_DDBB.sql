DROP DATABASE IF EXISTS juego;
CREATE DATABASE juego;
USE juego;

CREATE TABLE jugadores (
    ID_j INT PRIMARY KEY NOT NULL,
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
