app.ChangeTextInJSCallback('test',function(text){
    alert(text);
});

$(document).ready(function() {
    $('.btn').click(function() {
        window.app.ChangeTextInJS('Hello World!');
    });
});

function ChangeText(text) {
    $('#text').html(text);
}

