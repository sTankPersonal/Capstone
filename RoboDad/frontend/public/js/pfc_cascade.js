(function () {
    const primarySel  = document.getElementById('pfc_primary');
    const detailedSel = document.getElementById('pfc_detailed_category_id');
    if (!primarySel || !detailedSel) return;

    // Cache all detailed options by their data-primary attribute.
    const allOptions = Array.from(detailedSel.options).filter(o => o.value !== '');

    function filterDetailed(primaryId, selectValue) {
        // Remove all non-placeholder options first.
        while (detailedSel.options.length > 1) detailedSel.remove(1);

        const matches = primaryId
            ? allOptions.filter(o => o.dataset.primary === primaryId)
            : allOptions;

        matches.forEach(o => {
            const clone = o.cloneNode(true);
            if (clone.value === selectValue) clone.selected = true;
            detailedSel.appendChild(clone);
        });
    }

    primarySel.addEventListener('change', function () {
        filterDetailed(this.value, '');
    });

    // On edit pages, restore saved selection.
    const current = (typeof window.__pfcCurrentDetailed !== 'undefined')
        ? window.__pfcCurrentDetailed : '';

    if (current) {
        // Find what primary this detailed belongs to.
        const match = allOptions.find(o => o.value === current);
        if (match) {
            primarySel.value = match.dataset.primary;
            filterDetailed(match.dataset.primary, current);
        }
    } else {
        // Hide detailed until a primary is chosen.
        filterDetailed('', '');
    }
})();
