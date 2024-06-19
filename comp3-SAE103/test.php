#!/usr/bin/php
<?php
$file = file("DOC_UTILISATEUR.md");
file_put_contents('test.html','');

$listeOuverte = false;
$codeOuverte = false;


// Expression régulière pour extraire le texte encadré par des étoiles
$patternItaliqueGras = '/\*{1,3}(.*?)\*{1,3}/'; 

file_put_contents("test.html", "<!DOCTYPE html>\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"fr\" lang=\"fr\">\n<head>\n<meta charset=\"utf-8\" />\n<title>DOC_UTILISATEUR</title>\n</head>\n<body>\n", FILE_APPEND); 

foreach($file as $numLigne => $ligne ){

    $titre ='';
    $liste = '';
    $code = '';
    $lien='';
    $tableau ='';

# ***************** TITRES *****************
    # Titre H4
    if(str_starts_with($ligne, "#### ")){
        $titre = ltrim($ligne, $characters = "#### ");
        file_put_contents("test.html","<h4>$titre</h4>\n", FILE_APPEND);
    }

    # Titre H3
    elseif(str_starts_with($ligne, "### ")){
        $titre = ltrim($ligne, $characters = "### ");
        file_put_contents("test.html","<h3>$titre</h3>\n", FILE_APPEND);
    }

    # Titre H2
    elseif(str_starts_with($ligne, "## ")){
        $titre = ltrim($ligne, $characters = "## ");
        file_put_contents("test.html","<h2>$titre</h2>\n", FILE_APPEND);
    }

    # Titre H1
    elseif(str_starts_with($ligne, "# ")){
        $titre = ltrim($ligne, $characters = "# ");
        file_put_contents("test.html","<h1>$titre</h1>\n", FILE_APPEND);
    }

# ***************** LISTES *****************

    elseif(str_starts_with($ligne, "- ")) {
        $liste = ltrim($ligne, "- ");

        // Vérifier si la liste est ouverte
        if (!$listeOuverte) { 
            file_put_contents("test.html","<ul>\n", FILE_APPEND);
            $listeOuverte = true;
            if (preg_match_all($patternItaliqueGras, $liste, $matches)) {
                // $matches[1] contient le texte entre les étoiles
                foreach ($matches[1] as $texteEntreEtoiles) {
                    $texteGrasItalique = '<strong><em>' . $texteEntreEtoiles . '</em></strong>';
                    $texteGras = '<strong>' . $texteEntreEtoiles . '</strong>';
                    $texteItalique = '<em>' . $texteEntreEtoiles . '</em>';
                    
                    $liste = str_replace('***' . $texteEntreEtoiles . '***', $texteGrasItalique, $liste);
                    $liste = str_replace('**' . $texteEntreEtoiles . '**', $texteGras, $liste);
                    $liste = str_replace('*' . $texteEntreEtoiles . '*', $texteItalique, $liste);  
                }
                
            }
            file_put_contents("test.html","<li>$liste</li>\n", FILE_APPEND);
        }
        elseif($listeOuverte){
            $liste = ltrim($ligne, "- ");
            if (preg_match_all($patternItaliqueGras, $liste, $matches)) {
                // $matches[1] contient le texte entre les étoiles
                foreach ($matches[1] as $texteEntreEtoiles) {
                    $texteGrasItalique = '<strong><em>' . $texteEntreEtoiles . '</em></strong>';
                    $texteGras = '<strong>' . $texteEntreEtoiles . '</strong>';
                    $texteItalique = '<em>' . $texteEntreEtoiles . '</em>';
                    
                    $liste = str_replace('***' . $texteEntreEtoiles . '***', $texteGrasItalique, $liste);
                    $liste = str_replace('**' . $texteEntreEtoiles . '**', $texteGras, $liste);
                    $liste = str_replace('*' . $texteEntreEtoiles . '*', $texteItalique, $liste);  
                }
                
            }
            file_put_contents("test.html","<li>$liste</li>\n", FILE_APPEND);
            
        }
    }

        // Fermer la liste si elle est ouverte
    elseif((!str_starts_with($ligne, "- "))&& ($listeOuverte)){
        file_put_contents("test.html","</ul>\n", FILE_APPEND);
        $listeOuverte = false;
    }

    # ***************** BLOC CODE *****************

    elseif (str_starts_with($ligne, "```")&&(!$codeOuverte)) {
            $codeOuverte = true;
            file_put_contents("test.html", "<pre><code>\n", FILE_APPEND);
    }
    elseif (!str_starts_with($ligne, "```")&&($codeOuverte)){
        $code = $ligne;
        file_put_contents("test.html", htmlspecialchars($code), FILE_APPEND);
    }
    elseif( $codeOuverte && ((str_starts_with($ligne, "```")))){
        file_put_contents("test.html", htmlspecialchars($code) . "\n</code></pre>\n", FILE_APPEND);
        $codeOuverte = false;
    }

    elseif(preg_match('/\`(.*?)\`/', $ligne, $match)){
        $box = $match[1];
        $boxCode = "<code>$box</code>";
        $ligne = str_replace(htmlspecialchars("`$box`"), "$boxCode", $ligne, $count);
        file_put_contents("test.html","<p>$ligne</p>\n", FILE_APPEND);
    }
    # ***************** LIENS ***************** 
       
    elseif(preg_match('/\[(.*?)\]\((.*?)\)/', $ligne, $match)){
        $nom = $match[1];
        $link = $match[2];
        $lien = "<a href=\"$link\"> $nom </a>\n";
        $ligne = str_replace("[$nom]($link)", "$lien", $ligne, $count);
        file_put_contents("test.html","<p>$ligne</p>\n", FILE_APPEND);
    }

    # ***************** TABLEAU *****************   
        
    elseif (strpos($ligne, '|') !== false && strpos($ligne, '-') !== false) {
        $colonne = explode('|', trim($ligne));
        $tableau = '<tr><td>' . implode('</td><td>', $colonne) . '</td></tr>';
        file_put_contents("test.html", "<table>$tableau</table>\n", FILE_APPEND);
    }
    # ************* GRAS,ITALIC,GRASITALIC ***********

    elseif (preg_match_all($patternItaliqueGras, $ligne, $matches)) {
        // $matches[1] contient le texte entre les étoiles
        foreach ($matches[1] as $texteEntreEtoiles) {
            $texteGrasItalique = '<strong><em>' . $texteEntreEtoiles . '</em></strong>';
            $texteGras = '<strong>' . $texteEntreEtoiles . '</strong>';
            $texteItalique = '<em>' . $texteEntreEtoiles . '</em>';
            
            $ligne = str_replace('***' . $texteEntreEtoiles . '***', $texteGrasItalique, $ligne);
            $ligne = str_replace('**' . $texteEntreEtoiles . '**', $texteGras, $ligne);
            $ligne = str_replace('*' . $texteEntreEtoiles . '*', $texteItalique, $ligne);  
        }
        file_put_contents("test.html","<p>$ligne</p>\n", FILE_APPEND);
    }

    # ************* PARAGRAPHES,SPAN ***********
    else{
        if(rtrim($ligne,"\n")){
            file_put_contents("test.html","<p>$ligne</p>\n", FILE_APPEND);

        }
    }
        


}

?>