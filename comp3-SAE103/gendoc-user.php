#!/usr/bin/php
<?php
$file = file("DOC_UTILISATEUR.md");
file_put_contents('test.html','');

foreach($file as $numLigne => $ligne ){

    $titre ='';

    # Titre H4
    if(substr($ligne, 0, 4)=="####"){
        # commence au caractère à l'emplacement $i de la ligne jusqu'à la fin de la ligne (longueur).
        for($i = 4 ; $i < strlen($ligne); $i++){ 
            # concatener les caractères d'emplacement $i de la ligne dans la variable $titre.
            $titre = $titre.$ligne[$i];
        }
        # on enlève l'espace du début de la ligne (celui entre les dièses et le contenu de la ligne).
        $titre = trim($titre,' ');
        # on ajoute cette ligne dans un autre fichier .html avec les balises.
        file_put_contents("test.html","<h4>$titre</h4>\n", FILE_APPEND);
    }

    # Titre H3
    elseif(substr($ligne, 0, 3)=="###"){
        for($i = 3 ; $i < strlen($ligne); $i++){
            $titre = $titre.$ligne[$i];
        }
        $titre = trim($titre,' ');
        file_put_contents("test.html","<h3>$titre</h3>\n", FILE_APPEND);
    }

    # Titre H2
    elseif(substr($ligne, 0, 2)=="##"){
        for($i = 2 ; $i < strlen($ligne); $i++){
            $titre = $titre.$ligne[$i];
        }
        $titre = trim($titre,' ');
        file_put_contents("test.html","<h2>$titre</h2>\n", FILE_APPEND);
    }

    # Titre H1
    elseif(substr($ligne, 0, 1)=="#"){
        for($i = 2 ; $i < strlen($ligne); $i++){
            $titre = $titre.$ligne[$i];
        }
        $titre = trim($titre,' ');
        file_put_contents("test.html","<h1>$titre</h1>\n", FILE_APPEND);
    }

    # <em><b>BOLD ITALIC </b></em>

    $boldItalic = '';
    $etoile3 = "***";

    # on cherche le numéro de la position des astérix '***' dans la ligne
    $positionDeb = strpos($ligne ,$etoile3);
    
    # tant qu'il y a 3 astérix
    if($positionDeb !==false){

        # cherche si 2 eme occurence des 3 astérix
        $positionFin = strpos($ligne ,$etoile3,$positionDeb + 1);
        if($positionFin !== false){

            # on extrait les caractères situés entre les 2 occurences des 3 astérix
            $boldItalic = substr($ligne, $positionDeb + 3, (($positionFin - $positionDeb - 3)));
            
            # on remplace dans la ligne les éléments BoldItalic par le code en HTML, et on conte le nombre de remplacement
            $ligne = str_replace("***$boldItalic***", "<em><b>$boldItalic</b></em>", $ligne, $count);
            
            # on vérifie si quelque chose a bien été remplacé 
            #if ($count > 0) {
                file_put_contents("test.html",$ligne, FILE_APPEND); #on ajoute ce bout de code dans le HTML à la suite
                #}
        }

        #on remet la position du début à la prochaine occurrence d'astérix
        $positionDeb = strpos($ligne, $etoile3, $positionDeb + 1);
    
    }

    $etoile2 = "**";
    $positionDeb = strpos($ligne ,$etoile2);
    
    if($positionDeb !==false){

        $positionFin = strpos($ligne ,$etoile2, $positionDeb + 1);

        if($positionFin !== false){

            $bold = substr($ligne, $positionDeb + 2, (($positionFin - $positionDeb - 2)));
            $ligne = str_replace("**$bold**", "<b>$bold</b>", $ligne, $count);

            #if ($count > 0) {
                file_put_contents("test.html",$ligne, FILE_APPEND);
                #}
        }
        $positionDeb = strpos($ligne, $etoile2, $positionDeb + 1);
    
    }

    $italic = '';
    $etoile1 = "*";
    $positionDeb = strpos($ligne ,$etoile1);
    
    if($positionDeb !==false){

        $positionFin = strpos($ligne ,$etoile1,$positionDeb + 1);
        if($positionFin !== false){

            $italic = substr($ligne, $positionDeb + 1, (($positionFin - $positionDeb - 1)));
            $ligne = str_replace("*$italic*", "<em>$italic</em>", $ligne, $count);

            // Assure que le remplacement s'est effectué avant d'écrire dans le fichier
           # if ($count > 0) {
            file_put_contents("test.html",$ligne, FILE_APPEND);
            #}
    
        }

        $positionDeb = strpos($ligne, $etoile1, $positionDeb + 1);
    }
}
?>

