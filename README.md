
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
    <li><a href="#description">Description</a></li>
    <li><a href="#execution">Exécution</a>
      <ul>
        <li><a href="#partie-1-readelf">Partie 1</a></li>
        <li><a href="#partie-2">Partie 2</a></li>
      </ul>
    <li><a href="#roadmap">Roadmap</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## Description
**TO DO** DO WE NEED A DESCRIPTION HERE ?

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- Compilation and execution -->
## Execution

Le projet est divisé en deux parties. La <a href="#partie-1">premiere partie</a> contient les fonctions utilisées pour obtenir les informations dans un fichier ELF. 

**TODO** Part 2 

### Partie 1
L'objectif de la première partie est de recréer certaines fonctionnalités de readelf décrites ci-dessous. Le programme principale est écrite dans le fichier **readelf.c**. 

Nous avons également implémenté des options comme dans readelf afin de mieux visualiser les données en fonction de nos besoins. Afin de visualiser les <a href="#options">options</a> possibles, il suffit d'entrer :
    ```
    ./readelf
    ```

**Remarque** : les options doivent être utilisées avec un espace entre elles, par exemple : 
    
    ./readelf -h -S Examples_loader/example1.o

Dans cette exemple,  le programme affichera l'en-tête et la table des sections.

Voici des exemples pour chaque étape de la première partie:

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

* Affichage des tables de réimplantation et des détails relatifs à chaque entrée
    ```sh
    ./readelf -r Examples_loader/example1.o
    ```

#### Options 
Usage: ./readelf <option(s)> elf-file
  
Les options doivent être écrites séparément, ie: -h -S. Un exemple est fournie en haut.
     
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

### Partie 2 
TO DO ADD EXAMPLE EXECUTIONS ONCE COMPLETED
* Renumérotation des sections ;

* Correction des symboles ;

* Réimplantations de type R_ARM_ABS* ;

* Réimplantation de type R_ARM_JUMP24 et R_ARM_CALL ;

* Interfaçage avec le simulateur ARM ;

* Production d’un fichier exécutable non relogeable.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- ROADMAP -->
## Roadmap
This part is only temporary before we had in the project as a check list for this readme.

- [ ] Add Part 1 - readelf
- [ ] Add Part 2 - Reimplementation
    - [ ] step 6
    - [ ] step 7
    - [ ] step 8
    - [ ] step 9
    - [ ] step 10
    - [ ] step 11
- [ ] Complete CR 
- [ ] Complete Journal

<p align="right">(<a href="#top">back to top</a>)</p>

