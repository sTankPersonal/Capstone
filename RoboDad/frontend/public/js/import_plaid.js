(function () {
    var today = new Date();
    var firstOfMonth = new Date(today.getFullYear(), today.getMonth(), 1);
    document.getElementById('end_date').valueAsDate = today;
    document.getElementById('start_date').valueAsDate = firstOfMonth;
})();
