(function () {
    var btn = document.getElementById('import-csv-btn');
    var input = document.getElementById('import-csv-input');
    var status = document.getElementById('import-csv-status');
    if (!btn || !input) return;

    function showStatus(message, isError) {
        if (!status) return;
        status.textContent = message;
        status.hidden = false;
        status.classList.toggle('import-csv-status--error', !!isError);
    }

    btn.addEventListener('click', function () {
        input.click();
    });

    input.addEventListener('change', function () {
        var file = input.files && input.files[0];
        if (!file) return;

        var category = btn.dataset.category || 'expenses';
        var noun = btn.dataset.noun || 'transaction';
        showStatus('Importing ' + file.name + '…', false);
        btn.disabled = true;

        var reader = new FileReader();
        reader.onload = function () {
            fetch('/user/transactions/category/' + encodeURIComponent(category) + '/import/csv', {
                method: 'POST',
                headers: { 'Content-Type': 'text/csv' },
                body: reader.result
            }).then(function (res) {
                if (!res.ok) throw new Error('Server returned ' + res.status);
                return res.json();
            }).then(function (data) {
                var count = data && typeof data.imported === 'number' ? data.imported : 0;
                if (count > 0) {
                    showStatus('Imported ' + count + ' ' + noun + (count === 1 ? '' : 's') + '. Refreshing…', false);
                    window.location.reload();
                } else {
                    showStatus('No ' + noun + 's found in that file.', true);
                    btn.disabled = false;
                }
            }).catch(function (err) {
                showStatus('Import failed: ' + err.message, true);
                btn.disabled = false;
            });
        };
        reader.onerror = function () {
            showStatus('Could not read the selected file.', true);
            btn.disabled = false;
        };
        reader.readAsText(file);

        // Allow re-selecting the same file later.
        input.value = '';
    });
})();
