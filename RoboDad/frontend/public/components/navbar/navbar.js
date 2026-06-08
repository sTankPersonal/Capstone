document.addEventListener("DOMContentLoaded", async () => {
    const container = document.getElementById("navbar");
    if (!container) return;

    try {
        const htmlRes = await fetch("/components/navbar/navbar.html");
        container.innerHTML = await htmlRes.text();

        const cssLink = document.createElement("link");
        cssLink.rel = "stylesheet";
        cssLink.href = "/components/navbar/navbar.css";
        document.head.appendChild(cssLink);

        await new Promise((resolve, reject) => {
            const s = document.createElement('script');
            s.src = '/js/dropdowns.js';
            s.onload = resolve;
            s.onerror = reject;
            document.head.appendChild(s);
        });
        initializeNavDropdown({
            menuId: 'transactions-menu',
            apiUrl: '/references/transaction-categories',
            routeBuilder: id => `/user/transactions/category/${id}`
        });

        const path = window.location.pathname;
        container.querySelectorAll("a[href]").forEach(link => {
            const href = link.getAttribute("href");
            if (href && (path === href || path.startsWith(href + "/"))) {
                link.classList.add("active");
            }
        });

        const username = container.dataset.username;
        if (username) {
            const greeting = document.createElement("span");
            greeting.id = "nav-greeting";
            greeting.textContent = "Hello, " + username;
            container.querySelector(".nav-links").prepend(greeting);
        }

        const modal = document.createElement("div");
        modal.id = "logout-modal";
        modal.innerHTML = `
            <div id="logout-modal-box">
                <h3>Log out of RoboDad?</h3>
                <p>You will need to log back in to access your account.</p>
                <div id="logout-modal-actions">
                    <button id="logout-cancel">Cancel</button>
                    <button id="logout-confirm">Log Out</button>
                </div>
            </div>
        `;
        document.body.appendChild(modal);

        document.getElementById("logout-cancel").addEventListener("click", () => {
            modal.style.display = "none";
        });
        document.getElementById("logout-confirm").addEventListener("click", () => {
            window.location.href = "/auth/logout";
        });
        modal.addEventListener("click", (e) => {
            if (e.target === modal) modal.style.display = "none";
        });

        const logoutLink = container.querySelector("a[data-page='logout']");
        if (logoutLink) {
            logoutLink.addEventListener("click", (e) => {
                e.preventDefault();
                modal.style.display = "flex";
            });
        }

    } catch (err) {
        console.error("Failed to load navbar:", err);
    }
});
