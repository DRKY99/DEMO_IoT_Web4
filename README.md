Usage:

python3 main.py [VALUE> [INIT-DISTANCE]

PORT is set as default at port 80

python3 main.py [VALUE] [INIT-DISTANCE] [PORT]

Dependeces:

-   sqlite3
-   python3
    -   pysqlite3

Database schema:

-   id: int
-   value: varchar(10)
-   distance: float(8,2)
-   timestam: datetime
