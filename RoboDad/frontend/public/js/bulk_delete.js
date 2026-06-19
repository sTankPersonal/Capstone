(function () {
    var selectAll = document.getElementById('txn-select-all');
    var deleteBtn = document.getElementById('txn-delete-selected');
    var countEl = document.getElementById('txn-delete-count');
    if (!deleteBtn) return;

    function rowChecks() {
        return Array.prototype.slice.call(document.querySelectorAll('.txn-row-check'));
    }
    function isVisible(cb) {
        var row = cb.closest('.txn-row');
        return row && row.style.display !== 'none';
    }
    function visibleChecks() {
        return rowChecks().filter(isVisible);
    }
    function checkedChecks() {
        return rowChecks().filter(function (cb) { return cb.checked; });
    }

    // Keep the delete button + "select all" state in sync with the checkboxes.
    function refresh() {
        var count = checkedChecks().length;
        deleteBtn.disabled = count === 0;
        if (countEl) countEl.textContent = count ? '(' + count + ')' : '';

        if (selectAll) {
            var vis = visibleChecks();
            var visChecked = vis.filter(function (cb) { return cb.checked; }).length;
            if (vis.length === 0 || visChecked === 0) {
                selectAll.checked = false;
                selectAll.indeterminate = false;
            } else if (visChecked === vis.length) {
                selectAll.checked = true;
                selectAll.indeterminate = false;
            } else {
                selectAll.checked = false;
                selectAll.indeterminate = true;
            }
        }
    }

    function clearSelection() {
        rowChecks().forEach(function (cb) { cb.checked = false; });
        refresh();
    }

    if (selectAll) {
        selectAll.addEventListener('change', function () {
            var on = selectAll.checked;
            visibleChecks().forEach(function (cb) { cb.checked = on; });
            refresh();
        });
    }

    rowChecks().forEach(function (cb) {
        cb.addEventListener('change', refresh);
    });

    // When the search filter hides a row, drop it from the selection so that
    // "selected" never includes rows the user can't see.
    var searchInput = document.getElementById('txn-search-input');
    if (searchInput) {
        searchInput.addEventListener('input', function () {
            rowChecks().forEach(function (cb) {
                if (!isVisible(cb)) cb.checked = false;
            });
            refresh();
        });
    }

    // ---- Confirmation modal (mirrors the single-item delete card) ----
    var overlay = document.createElement('div');
    overlay.className = 'txn-modal-overlay';
    overlay.hidden = true;
    overlay.innerHTML =
        '<div class="delete-card" role="dialog" aria-modal="true" aria-labelledby="txn-modal-title">' +
            '<h2 id="txn-modal-title"></h2>' +
            '<p>This action cannot be undone.</p>' +
            '<div class="form-actions form-actions-center">' +
                '<button type="button" class="btn btn-danger" data-modal-confirm>Yes, Delete</button>' +
                '<button type="button" class="btn btn-secondary" data-modal-cancel>Cancel</button>' +
            '</div>' +
        '</div>';
    document.body.appendChild(overlay);

    var modalTitle = overlay.querySelector('#txn-modal-title');
    var confirmBtn = overlay.querySelector('[data-modal-confirm]');
    var cancelBtn = overlay.querySelector('[data-modal-cancel]');

    function closeModal() {
        overlay.hidden = true;
        document.removeEventListener('keydown', onKeydown);
    }
    function onKeydown(e) {
        if (e.key === 'Escape') closeModal();
    }
    function openModal(title, onConfirm) {
        modalTitle.textContent = title;
        overlay.hidden = false;
        document.addEventListener('keydown', onKeydown);
        confirmBtn.onclick = function () {
            closeModal();
            onConfirm();
        };
    }
    cancelBtn.addEventListener('click', closeModal);
    overlay.addEventListener('click', function (e) {
        if (e.target === overlay) closeModal();
    });

    function performDelete(ids) {
        var category = deleteBtn.dataset.category || 'expenses';
        deleteBtn.disabled = true;
        fetch('/user/transactions/category/' + encodeURIComponent(category) + '/delete', {
            method: 'POST',
            headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
            body: 'ids=' + encodeURIComponent(ids.join(','))
        }).then(function (res) {
            if (!res.ok) throw new Error('Server returned ' + res.status);
            return res.json();
        }).then(function () {
            clearSelection();        // deselect everything once the delete is done
            window.location.reload();
        }).catch(function (err) {
            window.alert('Delete failed: ' + err.message);
            deleteBtn.disabled = false;
        });
    }

    deleteBtn.addEventListener('click', function () {
        var ids = checkedChecks().map(function (cb) { return cb.value; });
        if (ids.length === 0) return;

        var noun = deleteBtn.dataset.noun || 'item';
        var label = ids.length + ' ' + noun + (ids.length === 1 ? '' : 's');
        openModal('Delete ' + label + '?', function () { performDelete(ids); });
    });

    refresh();
})();
