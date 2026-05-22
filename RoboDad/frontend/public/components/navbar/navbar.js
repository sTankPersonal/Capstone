document.addEventListener("DOMContentLoaded", async () => {
    const container = document.getElementById("navbar");
    if (!container) return;

    try {
        const htmlRes = await fetch("/assets/components/navbar/navbar.html");
        container.innerHTML = await htmlRes.text();

        const cssLink = document.createElement("link");
        cssLink.rel = "stylesheet";
        cssLink.href = "/assets/components/navbar/navbar.css";
        document.head.appendChild(cssLink);

        const current = window.location.pathname.replace("/", "");
        const links = container.querySelectorAll("a[data-page]");
        links.forEach(link => {
            if (link.dataset.page === current) {
                link.classList.add("active");
            }
        });

        // Fetch and display logged-in user's name
        try {
            const meRes = await fetch("/api/user/me");
            if (meRes.ok) {
                const me = await meRes.json();
                const greeting = document.createElement("span");
                greeting.id = "nav-greeting";
                greeting.textContent = "Hello, " + me.name;
                container.querySelector(".nav-links").prepend(greeting);
            }
        } catch (_) { }

        // Inject logout modal
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
            window.location.href = "/logout";
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