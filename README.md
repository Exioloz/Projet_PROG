
<!-- PROJECT Header -->
<br />
<div align="center">
  <h1 align="center">PROG 5 Projet 2021-2022</h1>

  <p align="center">
    Réalisation d'un éditeur de liens - Phase de réimplantation
    <br />
    <a href="https://github.com/Exioloz/Projet_PROG"><strong>Source code »</strong></a>
    <br />
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table des Matières</summary>
  <ol>
    <li><a href="#compilation">Compilation</a>
    <li><a href="#execution">Exécution</a>
      <ul>
        <li><a href="#phase-1-readelf">Phase 1</a></li>
        <li><a href="#phase-2">Phase 2</a></li>
      </ul>
    <li><a href="#testes">Testes</a></li>
  </ol>
</details>


<!-- Compilation -->
## Compilation
La compilation du code fourni se fait à l’aide d’un Makefile généré automatiquement lors de la configuration du projet. Pour compiler le programme, exécutez les instructions suivantes :
```sh
./configure

make
```

<!-- Execution -->
## Execution

Le projet est divisé en deux phases. La <a href="#phase-1">première phase</a> contient les fonctions utilisées pour obtenir les informations dans un fichier ELF. 
La <a href="#phase-2">deuxiéme phase</a> contient les fonctions utilisées pour 
modifier le contenu du fichier au format ELF donné afin d'effectuer l'implantation.

### Phase 1
L'objectif de la première phase consiste a recréer certaines fonctionnalités de readelf décrites ci-dessous. Le programme principal est écrit dans le fichier **readelf.c**. 

Nous avons également implémenté des options comme dans readelf afin de mieux visualiser les données en fonction de nos besoins. Afin de visualiser les <a href="#options">options</a> possibles, il suffit d'entrer :
    `
    ./readelf
    `

**Remarque** : les options doivent être utilisées avec un espace entre elles, par exemple : 
    
    ./readelf -h -S Examples_loader/example1.o

Dans cet exemple,  le programme affichera l'en-tête et la table des sections.

Voici des exemples pour chaque étape de la première phase:

* Affichage de l'en-tête
    ```sh
    ./readelf -h Examples_loader/example1.o
    ```
* Affichage de la table des sections et des détails relatifs à chaque section
    ```sh
    ./readelf -S Examples_loader/example1.o
    ```

* Affichage du contenu d'une section
    ```sh
    ./readelf -x ".text" Examples_loader/example1.o
    ```

* Affichage de la table des symboles et des détails relatifs à chaque symbole
    ```sh
    ./readelf -s Examples_loader/example1.o
    ```

* Affichage des tables de réimplémentation et des détails relatifs à chaque entrée
    ```sh
    ./readelf -r Examples_loader/example1.o
    ```

#### Options 
Usage: 
`
./readelf <option(s)> elf-file
`
  
Les options doivent être écrites séparément, ie : ` -h -S` . Un exemple est fourni en haut.
     
     Display information about the contents of ELF format files Options are:
      -a --all               Equivalent to: -h -S -s
      -e --headers           Equivalent to: -h -l -S
      -h --file-header       Display the ELF file header
      -S --section-headers   Display the sections' header 
         --sections          An alias for --section-headers
      -s --syms              Display the symbol table 
         --symbols           An alias for --syms
      -x --hex-dump=<name>                         
                             Dump the contents of section <name> as bytes
      -r --relocs            Display the relocations (if present)

<p align="right">(<a href="#top">back to top</a>)</p>

### Phase 2 
L'objectif de la deuxiéme phase est de modifier le contenu du fichier au format ELF donné afin 
d'effectuer l'implantation. Le programme prendr en paramètre les adresses auxquelles les sections 
du programme (.text et .data) doivent être chargées. 

Usage :
`
./relocation .text=0x**** .data=0x**** elf-file
`

Où ** est remplacé par les adresses auxquelles les sections du programme (.text et .data) doivent être chargées. 

Par exemple :
```sh
./relocation .text=0x20 .data=0x2800 Examples_loader/example4.o
```

<p align="right">(<a href="#top">back to top</a>)</p>

## Testes

Pour tester les étapes de phase 1 automaituqement, nous avons crée un programme qui éxécute tout les fichiers testes sur notre programme et produire un fichier de résultat nommé `resultat_tests.txt`.

Usage :
`./tests_part1.sh`

<p align="right">(<a href="#top">back to top</a>)</p>