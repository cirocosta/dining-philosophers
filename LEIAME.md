# Ep3 - Filósofos Faminitos c/ Monitores

## Executando

O EP foi implementado em C++ contando com o CMake como ferramenta de configuração automática.

```sh
$ mkdir build   # pasta com o resultado do build
$ cd build
$ cmake ..      # preparar os arquivos de make
$ make          # make
$./src/dinnp    # Usage (como abaixo) será mostrado)


## Exemplo:

$ mkdir build && cd buold && cmake .. && make && ./src/dinnp ../assets/poucos.txt 1000 p
```

Os arquivos fonte para o relatório se encontram em `./relatorio`.

### Usage

```sh
usage:
      $ ./main f R U|P

F     destination of the file specifying the
      number of philosophers and their weights

R     number of portions to be eaten

U|P   whether it should include weights or it
      runs uniformly
```

