(function () {
    var rows = Array.prototype.slice.call(document.querySelectorAll('.txn-row'));

    function fmt(n) {
        if (isNaN(n)) n = 0;
        return n.toLocaleString(undefined, { minimumFractionDigits: 2, maximumFractionDigits: 2 });
    }

    function setVal(id, text) {
        var el = document.getElementById(id);
        if (el) el.textContent = text;
    }

    var income = 0, expense = 0;

    // Style each row from its category, and tally the summary totals.
    rows.forEach(function (row) {
        var cat = (row.dataset.category || '').toLowerCase();
        var amtCell = row.querySelector('.txn-cell-amount');
        var amt = parseFloat(amtCell ? amtCell.dataset.amount : '');
        var badge = row.querySelector('.txn-badge');

        if (cat === 'earnings') {
            if (!isNaN(amt)) income += amt;
            if (amtCell) {
                amtCell.textContent = '+$' + fmt(amt);
                amtCell.classList.add('txn-amount--income');
            }
            if (badge) {
                badge.textContent = 'Income';
                badge.classList.add('txn-badge--income');
            }
        } else if (cat === 'expenses') {
            if (!isNaN(amt)) expense += amt;
            if (amtCell) {
                amtCell.textContent = '-$' + fmt(amt);
                amtCell.classList.add('txn-amount--expense');
            }
            if (badge) {
                badge.textContent = 'Expense';
                badge.classList.add('txn-badge--expense');
            }
        } else {
            if (amtCell && !isNaN(amt)) amtCell.textContent = '$' + fmt(amt);
            if (badge) badge.remove();
        }
    });

    var net = income - expense;
    setVal('income-value', '+$' + fmt(income));
    setVal('expense-value', '-$' + fmt(expense));
    setVal('net-value', (net < 0 ? '-$' : '$') + fmt(Math.abs(net)));
    // Single-type pages (expenses/earnings) only have one bucket, so the sum
    // of both equals that page's total.
    setVal('total-value', '$' + fmt(income + expense));
    setVal('count-value', String(rows.length));

    // ---- Filtering (type buttons + search) ----
    var filterBtns = Array.prototype.slice.call(document.querySelectorAll('.txn-filter-btn'));
    var searchInput = document.getElementById('txn-search-input');
    var activeFilter = 'all';

    function applyFilter() {
        var q = (searchInput && searchInput.value || '').trim().toLowerCase();
        var visible = 0;
        rows.forEach(function (row) {
            var cat = (row.dataset.category || '').toLowerCase();
            var desc = (row.dataset.description || '').toLowerCase();
            var matchType = activeFilter === 'all' || cat === activeFilter;
            var matchSearch = !q || desc.indexOf(q) !== -1;
            var show = matchType && matchSearch;
            row.style.display = show ? '' : 'none';
            if (show) visible++;
        });
        var empty = document.getElementById('txn-empty');
        if (empty) empty.style.display = visible === 0 ? '' : 'none';
    }

    filterBtns.forEach(function (btn) {
        btn.addEventListener('click', function () {
            filterBtns.forEach(function (b) { b.classList.remove('active'); });
            btn.classList.add('active');
            activeFilter = btn.dataset.filter;
            applyFilter();
        });
    });
    if (searchInput) searchInput.addEventListener('input', applyFilter);

    // ---- Per-row action menu ----
    document.querySelectorAll('.txn-menu-btn').forEach(function (btn) {
        btn.addEventListener('click', function (e) {
            e.stopPropagation();
            var list = btn.parentElement.querySelector('.txn-menu-list');
            var isOpen = list.classList.contains('open');
            document.querySelectorAll('.txn-menu-list.open').forEach(function (l) {
                l.classList.remove('open');
            });
            if (!isOpen) list.classList.add('open');
        });
    });
    document.addEventListener('click', function () {
        document.querySelectorAll('.txn-menu-list.open').forEach(function (l) {
            l.classList.remove('open');
        });
    });
})();
