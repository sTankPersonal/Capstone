(function () {
    var layout = document.querySelector('.dashboard-layout');
    if (!layout) return;

    var totalIncome = parseFloat(layout.dataset.totalIncome) || 0;
    var totalExpenses = parseFloat(layout.dataset.totalExpenses) || 0;

    function readCategoryItems(listId) {
        var items = [];
        document.querySelectorAll('#' + listId + ' li').forEach(function (li) {
            items.push({
                category: li.dataset.category,
                amount: parseFloat(li.dataset.amount) || 0,
                count: parseInt(li.dataset.count, 10) || 0
            });
        });
        return items;
    }

    var incomeItems = readCategoryItems('incomeDetailsList');
    var expenseItems = readCategoryItems('expenseDetailsList');

    var incomeLabels = incomeItems.map(function (i) { return i.category; });
    var incomeValues = incomeItems.map(function (i) { return i.amount; });
    var incomeCounts = incomeItems.map(function (i) { return i.count; });

    var expenseLabels = expenseItems.map(function (i) { return i.category; });
    var expenseValues = expenseItems.map(function (i) { return i.amount; });
    var expenseCounts = expenseItems.map(function (i) { return i.count; });

    // Charts are created lazily the first time their page becomes visible,
    // because Chart.js renders at zero size inside a display:none container.
    // Each chart MUST get its own fresh options object: Chart.js mutates the
    // options it is given (it writes resolved scale config back), so sharing one
    // object across instances leaks one chart's scales into another and breaks
    // the horizontal (indexAxis: 'y') charts.
    function baseOptions() {
        return { responsive: true, maintainAspectRatio: false };
    }

    var chartFactories = {
        overviewComparisonChart: function () {
            return new Chart(document.getElementById('overviewComparisonChart'), {
                type: 'bar',
                data: { labels: ['Income', 'Expenses'], datasets: [{ label: 'Total Amount', data: [totalIncome, totalExpenses] }] },
                options: baseOptions()
            });
        },
        incomeChart: function () {
            return new Chart(document.getElementById('incomeChart'), {
                type: 'doughnut',
                data: { labels: incomeLabels, datasets: [{ data: incomeValues }] },
                options: baseOptions()
            });
        },
        expenseChart: function () {
            return new Chart(document.getElementById('expenseChart'), {
                type: 'doughnut',
                data: { labels: expenseLabels, datasets: [{ data: expenseValues }] },
                options: baseOptions()
            });
        },
        expenseBarChart: function () {
            var options = baseOptions();
            options.indexAxis = 'y';
            return new Chart(document.getElementById('expenseBarChart'), {
                type: 'bar',
                data: { labels: expenseLabels, datasets: [{ data: expenseValues, label: 'Expenses' }] },
                options: options
            });
        },
        incomeBarChart: function () {
            var options = baseOptions();
            options.indexAxis = 'y';
            return new Chart(document.getElementById('incomeBarChart'), {
                type: 'bar',
                data: { labels: incomeLabels, datasets: [{ data: incomeValues, label: 'Income' }] },
                options: options
            });
        },
        incomeExpenseComparisonChart: function () {
            return new Chart(document.getElementById('incomeExpenseComparisonChart'), {
                type: 'bar',
                data: { labels: ['Income', 'Expenses'], datasets: [{ label: 'Total Amount', data: [totalIncome, totalExpenses] }] },
                options: baseOptions()
            });
        },
        expenseFrequencyChart: function () {
            return new Chart(document.getElementById('expenseFrequencyChart'), {
                type: 'bar',
                data: { labels: expenseLabels, datasets: [{ label: 'Number of Transactions', data: expenseCounts }] },
                options: baseOptions()
            });
        },
        incomeFrequencyChart: function () {
            return new Chart(document.getElementById('incomeFrequencyChart'), {
                type: 'bar',
                data: { labels: incomeLabels, datasets: [{ label: 'Number of Transactions', data: incomeCounts }] },
                options: baseOptions()
            });
        }
    };

    var createdCharts = {};

    // Which chart canvas ids belong to each page.
    var pageCharts = {
        overview: ['overviewComparisonChart'],
        incomeBreakdown: ['incomeChart'],
        expenseBreakdown: ['expenseChart'],
        expenseCategories: ['expenseBarChart'],
        incomeCategories: ['incomeBarChart'],
        incomeVsExpenses: ['incomeExpenseComparisonChart'],
        expenseFrequency: ['expenseFrequencyChart'],
        incomeFrequency: ['incomeFrequencyChart']
    };

    function buildChartsFor(target) {
        var ids = pageCharts[target];
        if (!ids) return;
        ids.forEach(function (id) {
            if (!createdCharts[id] && chartFactories[id]) {
                createdCharts[id] = chartFactories[id]();
            }
        });
    }

    function showPage(target) {
        document.querySelectorAll('.dash-page').forEach(function (p) { p.classList.remove('active'); });
        document.querySelectorAll('.dash-link').forEach(function (l) { l.classList.remove('active'); });

        var page = document.getElementById('page-' + target);
        if (page) page.classList.add('active');

        var link = document.querySelector('.dash-link[data-target="' + target + '"]');
        if (link) link.classList.add('active');

        buildChartsFor(target);
    }

    document.querySelectorAll('.dash-link').forEach(function (link) {
        link.addEventListener('click', function () { showPage(link.dataset.target); });
    });

    // Build the default (overview) chart on load.
    buildChartsFor('overview');
})();
