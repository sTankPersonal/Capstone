async function populateDropdown(selectId, endpoint, dataKey, selectedValue = '', labelKey = 'value', descriptionKey = '') {
    const select = document.getElementById(selectId);
    if (!select) return;
    // Native <option> text cannot be partially styled, so descriptions are
    // shown in a separate "<selectId>-description" element below the select.
    const descriptionEl = descriptionKey ? document.getElementById(selectId + '-description') : null;
    try {
        const res = await fetch(endpoint);
        if (!res.ok) throw new Error('Failed to load');
        const data = await res.json();
        select.innerHTML = '<option value="">-- Select --</option>';
        for (const item of data[dataKey]) {
            const opt = document.createElement('option');
            opt.value = item.id;
            opt.textContent = descriptionKey && item[descriptionKey]
                ? item[labelKey] + ' — ' + item[descriptionKey]
                : item[labelKey];
            if (descriptionKey && item[descriptionKey]) {
                opt.dataset.description = item[descriptionKey];
            }
            if (item.id === selectedValue) opt.selected = true;
            select.appendChild(opt);
        }
        if (descriptionEl) {
            const updateDescription = () => {
                const opt = select.selectedOptions[0];
                descriptionEl.textContent = (opt && opt.dataset.description) || '';
            };
            select.addEventListener('change', updateDescription);
            updateDescription();
        }
    } catch {
        select.add(new Option('Failed to load', ''));
    }
}

/*
 * Rich replacement for populateDropdown: hides the native <select> (which
 * still carries the value on form submission) and renders a custom listbox
 * where each option shows its label with the description in gray beneath it.
 * Expects an optional "<selectId>-description" element below the select and
 * a <label for="<selectId>"> whose target is retargeted to the trigger.
 */
async function populateCustomDropdown(selectId, endpoint, dataKey, selectedValue = '', labelKey = 'value', descriptionKey = 'description') {
    const select = document.getElementById(selectId);
    if (!select) return;
    const descriptionEl = document.getElementById(selectId + '-description');

    let items;
    try {
        const res = await fetch(endpoint);
        if (!res.ok) throw new Error('Failed to load');
        const data = await res.json();
        items = data[dataKey];
    } catch {
        select.innerHTML = '';
        select.add(new Option('Failed to load', ''));
        return;
    }

    // The hidden select keeps the form submission contract unchanged.
    select.innerHTML = '<option value=""></option>';
    for (const item of items) {
        select.add(new Option(item[labelKey], item.id));
    }
    select.value = selectedValue;
    select.hidden = true;
    select.tabIndex = -1;
    select.setAttribute('aria-hidden', 'true');

    const wrapper = document.createElement('div');
    wrapper.className = 'custom-select';

    const trigger = document.createElement('button');
    trigger.type = 'button';
    trigger.id = selectId + '-trigger';
    trigger.className = 'custom-select-trigger placeholder';
    trigger.setAttribute('aria-haspopup', 'listbox');
    trigger.setAttribute('aria-expanded', 'false');
    trigger.textContent = '-- Select --';

    const label = document.querySelector('label[for="' + selectId + '"]');
    if (label) label.setAttribute('for', trigger.id);

    const list = document.createElement('ul');
    list.className = 'custom-select-list';
    list.setAttribute('role', 'listbox');
    list.hidden = true;

    const optionEls = items.map((item) => {
        const li = document.createElement('li');
        li.className = 'custom-select-option';
        li.setAttribute('role', 'option');
        li.setAttribute('aria-selected', 'false');

        const name = document.createElement('span');
        name.className = 'option-name';
        name.textContent = item[labelKey];
        li.appendChild(name);

        if (item[descriptionKey]) {
            const desc = document.createElement('span');
            desc.className = 'option-description';
            desc.textContent = item[descriptionKey];
            li.appendChild(desc);
        }
        list.appendChild(li);
        return li;
    });

    wrapper.appendChild(trigger);
    wrapper.appendChild(list);
    select.insertAdjacentElement('afterend', wrapper);

    let activeIndex = items.findIndex((item) => item.id === selectedValue);

    function setActive(index) {
        activeIndex = index;
        optionEls.forEach((el, i) => el.classList.toggle('active', i === index));
        optionEls[index].scrollIntoView({ block: 'nearest' });
    }

    function applySelection(index) {
        activeIndex = index;
        const item = items[index];
        select.value = item.id;
        trigger.textContent = item[labelKey];
        trigger.classList.remove('placeholder', 'error');
        optionEls.forEach((el, i) => el.setAttribute('aria-selected', String(i === index)));
        if (descriptionEl) descriptionEl.textContent = item[descriptionKey] || '';
    }

    function open() {
        list.hidden = false;
        trigger.setAttribute('aria-expanded', 'true');
        setActive(activeIndex >= 0 ? activeIndex : 0);
    }

    function close() {
        list.hidden = true;
        trigger.setAttribute('aria-expanded', 'false');
    }

    if (activeIndex >= 0) applySelection(activeIndex);

    trigger.addEventListener('click', () => {
        if (list.hidden) open(); else close();
    });

    trigger.addEventListener('keydown', (e) => {
        if (e.key === 'ArrowDown' || e.key === 'ArrowUp') {
            e.preventDefault();
            if (list.hidden) {
                open();
            } else {
                const delta = e.key === 'ArrowDown' ? 1 : -1;
                const next = Math.min(Math.max(activeIndex + delta, 0), optionEls.length - 1);
                setActive(next);
            }
        } else if ((e.key === 'Enter' || e.key === ' ') && !list.hidden) {
            // preventDefault also stops the button's synthesized click,
            // which would otherwise immediately re-toggle the list.
            e.preventDefault();
            applySelection(activeIndex);
            close();
        } else if (e.key === 'Escape' && !list.hidden) {
            close();
        }
    });

    optionEls.forEach((el, i) => {
        el.addEventListener('mouseenter', () => setActive(i));
        el.addEventListener('click', () => {
            applySelection(i);
            close();
            trigger.focus();
        });
    });

    document.addEventListener('click', (e) => {
        if (!wrapper.contains(e.target)) close();
    });

    // The hidden select cannot use the required attribute (invisible controls
    // break native validation), so enforce the selection here.
    if (select.form) {
        select.form.addEventListener('submit', (e) => {
            if (!select.value) {
                e.preventDefault();
                trigger.classList.add('error');
                trigger.focus();
            }
        });
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
