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
Use: kflex options
Options: 
   count <Preprocessing data and count>
   search <Search specific kmer and its counts>
   dump <Extract hashmap of all kmers and its counts>
   batch <Searching for many kmers>
   dsearch <Searching for many kmers directly>
   kdive <Searching for many kmers with d mutations>
   khmap <Extract hashmap for k less or equal to kmax>

