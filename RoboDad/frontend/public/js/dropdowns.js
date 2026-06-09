async function populateDropdown(selectId, endpoint, dataKey, selectedValue = '', labelKey = 'value') {
    const select = document.getElementById(selectId);
    if (!select) return;
    try {
        const res = await fetch(endpoint);
        if (!res.ok) throw new Error('Failed to load');
        const data = await res.json();
        select.innerHTML = '<option value="">-- Select --</option>';
        for (const item of data[dataKey]) {
            const opt = document.createElement('option');
            opt.value = item.id;
            opt.textContent = item[labelKey];
            if (item.id === selectedValue) opt.selected = true;
            select.appendChild(opt);
        }
    } catch {
        select.add(new Option('Failed to load', ''));
    }
}

async function initializeNavDropdown({ menuId, apiUrl, routeBuilder }) {
    const menu = document.getElementById(menuId);
    if (!menu) return;
    try {
        const res = await fetch(apiUrl);
        if (!res.ok) throw new Error('Failed to load');
        const data = await res.json();
        const items = Object.values(data)[0];
        menu.innerHTML = '';
        for (const { id, value } of items) {
            const a = document.createElement('a');
            a.href = routeBuilder(id);
            a.textContent = value;
            menu.appendChild(a);
        }
    } catch {
        const span = document.createElement('span');
        span.textContent = 'Failed to load';
        menu.appendChild(span);
    }
}
