FROM postgres:16-alpine

# init.sql is executed automatically by the official postgres image
# on first startup when the data directory is empty.
COPY init.sql /docker-entrypoint-initdb.d/init.sql