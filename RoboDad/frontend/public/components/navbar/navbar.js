document.addEventListener("DOMContentLoaded", async () => {
    const container = document.getElementById("navbar");
    if (!container) return;

    try {
        // Load HTML
        const htmlRes = await fetch("/assets/components/navbar/navbar.html");
        container.innerHTML = await htmlRes.text();

        // Load CSS
        const cssLink = document.createElement("link");
        cssLink.rel = "stylesheet";
        cssLink.href = "/assets/components/navbar/navbar.css";
        document.head.appendChild(cssLink);

        // Highlight active page
        const current = window.location.pathname.replace("/", "");
        const links = container.querySelectorAll("a[data-page]");
        links.forEach(link => {
            if (link.dataset.page === current) {
                link.classList.add("active");
            }
        });

        // Logout: clear token then navigate to login
        const logoutLink = container.querySelector("a[data-page='logout']");
        if (logoutLink) {
            logoutLink.addEventListener("click", async (e) => {
                e.preventDefault();
                await fetch("/api/auth/logout", { method: "POST" }).catch(() => {});
                localStorage.removeItem("token");
                window.location.href = "/login";
            });
        }

    } catch (err) {
        console.error("Failed to load navbar:", err);
    }
});
