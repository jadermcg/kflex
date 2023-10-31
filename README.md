![kflex](https://github.com/jadermcg/kflex/blob/4812a42c994811125cb2acc704c88c3b1096222f/logo.png)
# KFLEX: A Fast and Efficient Framework for Counting and Manipulating kmers
KFLEX is an advanced Linux/C++ framework designed for efficiently handling kmers in large genomic datasets. Using **Sparse Motif Tree (SMT)** (for more information about what is SMT, click [here](https://github.com/jadermcg/smt)) as its basic infrastructure, kflex was designed to optimize both RAM memory usage and processing time.

One of KFLEX's most notable features is its ability to work with large volumes of data without sacrificing efficiency. This is achieved through the use of kmer minimizers, which allow for a compact and effective representation of kmers. Additionally, the SMT structure contributes to rapid storage and retrieval of kmers, making KFLEX a robust solution for applications that require high performance and efficiency.

Whether you are a researcher, a bioinformatician or a developer, KFLEX offers a reliable and efficient solution for all your kmer-related needs.

## Introduction

## Instalation

### Prerequisites and dependencies
O programa `kflex` foi projetado para ser executado no sistema operacional Linux e possui as seguintes dependências:

- **GLIBCXX_3.4.29** e **GLIBC_2.34**: Estas são versões específicas das bibliotecas libstdc++ (parte da biblioteca padrão C++) e glibc (biblioteca C padrão no Linux). Certifique-se de que sua distribuição Linux tenha as versões apropriadas instaladas.
- **Boost Filesystem**: Uma biblioteca do Boost para operações de sistema de arquivos.
- **Intel TBB**: Threading Building Blocks, uma biblioteca que suporta a paralelização e execução eficiente de programas.
- **Bibliotecas padrão do Linux**: libstdc++.so.6, libm.so.6, libgcc_s.so.1, e libc.so.6.

### Installation Steps
- Primeiro, instale as dependências. Se você estiver usando o sistema operacional Linux/Ubuntu, poderá executar o comando abaixo. Para outras distribuições Linux, será necessário consultar a documentação do sistema
  ```
  sudo apt update && sudo apt install libstdc++6 libgcc1 libc6 libboost-filesystem-dev libtbb-dev
  ```
## Problemas
- Se ao executar o kflex, o sistema retornar o erro xxx, execute o seguinte comando: ```sudo ln -s /usr/lib/x86_64-linux-gnu/libtbb.so /usr/lib/x86_64-linux-gnu/libtbb.so.12```

## Use
