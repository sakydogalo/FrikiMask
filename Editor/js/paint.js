$( document ).ready(function() {

    $(".color").click(function(event) {
        $("#chosen").removeAttr('id');
        this.id = 'chosen';
    });

    $("#canvas").on("click", ".cell", function() {				
        colorClass = $(this).attr("class").split(" ").pop();
        //swap the pixel color
        if(colorClass=="x_00") $(this).removeClass(colorClass).addClass("x_FF")
        else $(this).removeClass(colorClass).addClass("x_00")

        $(this).text( "" );
        
    });

});

var setCode = function(html) {
    //Obtenemos filas
    var filas = document.getElementsByClassName('row');
    var j = 0;
    var code = _.reduce(filas, function(matriz, fila) {
        j+=1;
        if (j==1) matriz+= "{ 0b";
        else matriz+=" ,0b";
        //Obtenemos columnas de una fila
        var columnas = fila.getElementsByClassName('cell');
        var i = 0;
        //Procesamos las celdas de una linea
        var linea = _.reduce(columnas, function(memo2, cell) {
            var celda = cell.className.split(" ").pop().split("_").pop();
            //console.log("Contador i"+ i);
            if (celda=="FF") memo2 +="0";
            else memo2 +="1";
            i+=1;
            return memo2;
            }, "");
        //Añadimos la linea a la matriz 

        matriz+= linea + " ";
        if (j==filas.length) matriz+= "}";

        return matriz;
        
    }
    , "");

    //console.log("Resultado");
    //console.log(code);
    document.getElementById("code").innerHTML = code;
};

var generateDivs = function(html) {
    event.preventDefault();
    var rows = $("#inputRows").val();
    var cols = $("#inputCols").val();


    if (rows === "") {
        rows = 8;
    } 
    if (cols === "") {
        cols = 8;
    }
    rows=8;
    cols=8;


    var htmlString = "";
    for (i = 0; i < rows; i++) {
        htmlString += "<div class=\"row\">";
        for (j = 0; j < cols; j++) {
            htmlString += "<div class=\"cell x_FF\" id=\"d" + i + j + "\"></div>";
        }
        htmlString += "</div>";
    }
    //console.log(htmlString);

    document.getElementById("canvas").innerHTML = htmlString;
}

function bodyCargado() {
    generateDivs(this);
}
