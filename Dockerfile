FROM gcc:4.9
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN make clean
RUN make all
CMD ["./calhyd","hull/hull_fin.csv","conf_fin.csv"]