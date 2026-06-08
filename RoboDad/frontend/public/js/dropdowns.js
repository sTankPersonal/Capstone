async function populateDropdown(selectId, endpoint, dataKey, labelKey = 'value') {
    const select = document.getElementById(selectId);
    if (!select) return;
    try {
        const res = await fetch(endpoint);
        if (!res.ok) throw new Error('Failed to load');
        const data = await res.json();
        select.innerHTML = '';
        for (const item of data[dataKey]) {
            const opt = document.createElement('option');
            opt.value = item.id;
            opt.textContent = item[labelKey];
            select.appendChild(opt);
        }
    } catch {
        select.innerHTML = '<option value="">Failed to load options</option>';
    }
}
