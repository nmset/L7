CREATE TABLE "List1" (
    "ID_List1" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    "Item" TEXT NOT NULL
);
CREATE TABLE tbl2 (
    "PK2" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    "PK1" INTEGER NOT NULL,
    "text2" TEXT NOT NULL,
    "check2d" INTEGER,
    "check2t" INTEGER,
    "combo2t" INTEGER,
    "combo2x" TEXT,
    "date2" TEXT,
    "spin2" INTEGER DEFAULT 5,
    "rand1" INTEGER NOT NULL DEFAULT (abs(random())),
    "json2" TEXT,
    "xml2" TEXT,
    foreign key (combo2t) references list1(id_list1),
    foreign key (pk1) references tbl1(pk1)
);
CREATE TABLE tbl1 (
    "PK1" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    "text1" TEXT NOT NULL,
    "check1d" INTEGER,
    "check1t" INTEGER,
    "combo1t" INTEGER,
    "combo1x" TEXT,
    "date1" TEXT,
    "spin1" INTEGER DEFAULT 3,
    "rand1" INTEGER NOT NULL DEFAULT (abs(random())),
    "json1" TEXT,
    "xml1" TEXT
);

INSERT INTO List1 VALUES(1, 'List1Item1');
INSERT INTO List1 VALUES(2, 'List1Item2');
INSERT INTO List1 VALUES(3, 'List1Item3');
INSERT INTO List1 VALUES(4, 'List1Item4');
INSERT INTO List1 VALUES(5, 'List1Item5');
