(function () {
    var total = 0, count = 0;
    document.querySelectorAll('.amount-cell').forEach(function (cell) {
        var val = parseFloat(cell.textContent.replace('$', ''));
        if (!isNaN(val)) { total += val; count++; }
    });
    document.getElementById('total-value').textContent = '$' + total.toFixed(2);
    document.getElementById('count-value').textContent = count;
})();
