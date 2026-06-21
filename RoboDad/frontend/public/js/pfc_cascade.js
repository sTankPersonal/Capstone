(function () {
    const primarySel  = document.getElementById('pfc_primary');
    const detailedSel = document.getElementById('pfc_detailed_category_id');
    if (!primarySel || !detailedSel) return;

    let allDetailed = [];

    function filterDetailed(primaryId, selectValue) {
        while (detailedSel.options.length > 1) detailedSel.remove(1);

        const matches = primaryId
            ? allDetailed.filter(d => d.primaryCategoryId === primaryId)
            : allDetailed;

        matches.forEach(d => {
            const option = document.createElement('option');
            option.value = d.id;
            option.dataset.primary = d.primaryCategoryId;
            option.textContent = d.value;
            if (d.id === selectValue) option.selected = true;
            detailedSel.appendChild(option);
        });
    }

    Promise.all([
        fetch('/references/pfc-primary-categories').then(r => r.json()),
        fetch('/references/pfc-detailed-categories').then(r => r.json())
    ]).then(([primaryData, detailedData]) => {
        const primaries = primaryData.pfcPrimaryCategories || [];
        allDetailed = detailedData.pfcDetailedCategories || [];

        primaries.forEach(p => {
            const option = document.createElement('option');
            option.value = p.id;
            option.textContent = p.value;
            primarySel.appendChild(option);
        });

        primarySel.addEventListener('change', function () {
            filterDetailed(this.value, '');
        });

        // On edit pages, restore saved selection.
        const current = (typeof window.__pfcCurrentDetailed !== 'undefined')
            ? window.__pfcCurrentDetailed : '';

        if (current) {
            const match = allDetailed.find(d => d.id === current);
            if (match) {
                primarySel.value = match.primaryCategoryId;
                filterDetailed(match.primaryCategoryId, current);
            }
        } else {
            // Hide detailed until a primary is chosen.
            filterDetailed('', '');
        }
    });
})();
