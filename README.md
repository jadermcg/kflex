![kflex](https://github.com/jadermcg/kflex/blob/4812a42c994811125cb2acc704c88c3b1096222f/logo.png)
# KFLEX: A Fast and Efficient Framework for Counting and Manipulating kmers
KFLEX is an advanced Linux/C++ framework designed for efficiently handling kmers in large genomic datasets. Using **Sparse Motif Tree (SMT)** (for more information about what is SMT, click [here](https://github.com/jadermcg/smt)) as its basic infrastructure, kflex was designed to optimize both RAM memory usage and processing time.

One of KFLEX's most notable features is its ability to work with large volumes of data without sacrificing efficiency. This is achieved through the use of kmer minimizers, which allow for a compact and effective representation of kmers. Additionally, the SMT structure contributes to rapid storage and retrieval of kmers, making KFLEX a robust solution for applications that require high performance and efficiency.

Whether you are a researcher, a bioinformatician or a developer, KFLEX offers a reliable and efficient solution for all your kmer-related needs.

## Introduction

## Instalation
Os seguintes passos devem ser obedecidos para a correta instalação do KFLEX.

### Prerequisites and dependencies
O programa `kflex` foi projetado para ser executado no sistema operacional Linux e possui as seguintes dependências:

- **GLIBCXX_3.4.29** e **GLIBC_2.34**: Estas são versões específicas das bibliotecas libstdc++ (parte da biblioteca padrão C++) e glibc (biblioteca C padrão no Linux). Certifique-se de que sua distribuição Linux tenha as versões apropriadas instaladas.
- **Boost Filesystem**: Uma biblioteca do Boost para operações de sistema de arquivos.
- **Intel TBB**: Threading Building Blocks, uma biblioteca que suporta a paralelização e execução eficiente de programas.
- **Bibliotecas padrão do Linux**: libstdc++.so.6, libm.so.6, libgcc_s.so.1, e libc.so.6.

### Installation Steps
O KFLEX pode ser utilizado diretamente através do arquivo binário presenta no diretório ```bin```ou compilado através do código fonte.

## Instalação através do arquivo binário
Os comandos abaixo foram testados no sistema operacional Ubuntu 22.04 LTS 64 bits. Para outros sistemas, por gentileza, consultar a documentação correspondente.
1) Primeiro, instale as dependências com a seguinte instrução: ``` sudo apt update && sudo apt install libstdc++6 libgcc1 libc6 libboost-filesystem-dev libtbb-dev```.
2) Crie um diretório para armazenar o binário do KFLEX, por exemplo: ```mkdir /caminho/para/klex```.
3) Faça download do binário do kflex presente no diretório bin do GitHub e salve no diretório que foi criado acima.
4) Coloque o diretório acima no path para que ele possa ser executado de qualquer parte do sistema: ``` echo "export PATH=$PATH:/caminho/para/kflex" ```
  
## Instalação através da compilação do código fonte
Para instalar o KFLEX através do código fonte, faça download dos arquivos presentes no diretório ```source``` para um diretório local no seu computador. Após isso, basta executar o comando ``` make ```. Isso irá gerar um executável chamado ```kflex```. Após isso, basta seguir os 1 até 4, conforme foi explicado logo acima.

## Exemplo de uso
Abaixo segue um exemplo que mostra como utilizar o programa de forma básica.

```
Use: kflex options
Options: 
   count <Preprocessing data and count>
   search <Search specific kmer and its counts>
   dump <Extract hashmap of all kmers and its counts>
   batch <Searching for many kmers>
   dsearch <Searching for many kmers directly>
   kdive <Searching for many kmers with d mutations>
   khmap <Extract hashmap for k less or equal to kmax>
```

Primeiro, precisamos criar o banco de dados através do comando: ``` kflex count -i <fasta file> -k <kmer size> -p <sufix size> ```.
Após isso, um diretório chamado *flex_data* será criado no mesmo diretório no qual o KFLEX foi executado. Após isso, podemos fazer a a extração dos kmers através da instrução: ``` kflex dump ```. Isso criará um arquivo chamado *hmap.txt* dentro de *flex_data* com a contagem de todos os kmers. Além disso, é possível realizar pesquisas com o ``` kflex search -kmer <kmer to search> ```. O comando ```kflex batch -f <path to kmer> ``` faz o mesmo que o comando anterior, porém pode realizar a pesquisa em mais de um kmer ao mesmo tempo. O comando ``` kflex kdive -f <path to kmers> -d <umber of mutations>  ``` é particularmente últil para a análise e descoberta de motifs biológicos, pois permite pesquisar kmers com até *d* mutações. Por fim o ```khmap -k <size to kmer> ``` permite que hashmaps de tamanhos menores do que o valor de *k* empregado inicialmente para criar o bando de dados de kmers sejam extraídos.

Por exemplo, considere o dataset de exemplo 200000.fasta. Este dataset possui um total de 200 mil sequências com largura de 100pb cada. Para criar o banco de dados inicial, podemos executar:
```
kflex count -i 200000.fasta -k 30 -p 5
```
Isso criará um diretório *flex_data* e no qual será processado o banco de dados de kmers. Após isso, podemos extrair o mapa para todos os kmers de tamanho 30:

```
kflex dump
```
O comando acima criará um arquivo chamado *hmap.txt* com todos os kmers e suas respectivas contagens. Podemos ainda extrair mapas com valores de *k* entre 5 e 29. O comando abaixo dá o exemplo para *k = 18*:

```
kflex khmap -k 18
```

Podemos consultar também se um kmer se encontra no banco de dados. Para isso, usamos:

```
kflex search AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
```
Neste caso, será consultado se o 30-mer *AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA* está presente no dataset. Se sim, o programa retornará o kmer e sua contagem. Caso contrário, o valor retornado será zero.

Podemos também consultar vários kmers ao mesmo tempo. Para isso, primeiro precisamos criar um arquivo com o kmers a serem pesquisados. Considere a instrução abaixo:

```
sort -k2nr < flex_data/hmap.txt | head -n 10 | awk '{print $1}' > flex_data/kmers.txt
```
Este comando extrairá do mapa *hmap.txt* os 10 kmers mais frequentes e criará um arquivo em flex_data chamado *kmers.txt*. Podemos usar este arquivo como entrada para o comando de busca em batch:

```
kflex batch -f flex_data/kmers.txt
```

O comando retornará os kmers e seus respectivas contagens.

Por fim, principalmente para a área de análise e descoberta de motifs biológicos, podemos buscar todos os kmers com até *d* mutações. Este comando também faz busca em lote, portanto precisamos criar um arquivo com os kmers a serem pesquisados. Vamos usar o arquivo *flex_data/kmers.txt* criado anteriormente. O comando para buscar considerando *d* mutações é o seguinte:

```
kflex kdive -f flex_data/kmers.txt -d 2
```

O comando acima buscará todos os kmers com até *d* mutações considerando todos os kmers pertencentes a *kmers.txt*. O algoritmo irá criar um diretório dentro de *flex_data* chamado *kdive_dir*. Dentro deste diretório, serão criados vários arquivos, um para cada kmer dentro de *kmers.txt*. Dentro deste arquivos, serão colocados todos os kmers com até *d* mutações. Esses dados podem ser usados posteriormente para criar modelos probabilísticos e ajudando assim na busca por motifs biologicamente releventes.
