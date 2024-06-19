$listeOuverte = false;
    $liste="";
    if(str_starts_with($ligne, "- ")){
        if($listeOuverte = false){
            $liste = ltrim($ligne, $characters = "- ");
            file_put_contents("test.html","<ul>\n", FILE_APPEND);
            file_put_contents("test.html","<li>$liste</li>\n", FILE_APPEND);
            $listeOuverte = true;
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

    }

    $positionDeb = strpos($ligne, $etoile1, $positionDeb + 1);
}
}

foreach($file as $numLigne => $ligne ){

# ***************** BLOC CODE *****************

$code = "";

if (str_starts_with($ligne, "```")) {
$codeOuverte = true;
if ($codeOuverte) {
    file_put_contents("test.html", "<pre>\n<code>\n", FILE_APPEND);
}

}
if($codeOuverte && !(str_starts_with($ligne, "```"))){
$code = next($ligne);
file_put_contents("test.html", htmlspecialchars($ligne) . "\n", FILE_APPEND);
}
else{
$codeOuverte=false;
if(!$codeOuverte && (str_starts_with($ligne, "```"))){
    file_put_contents("test.html","</code>\n</pre>\n", FILE_APPEND);
}
}
        
    