// ------------------------------------------------------
// DOM Elements
// ------------------------------------------------------
const chat = document.getElementById("chat");
const input = document.getElementById("messageInput");
const sendBtn = document.getElementById("sendBtn");
const sessionSelect = document.getElementById("sessionSelect");
const newSessionBtn = document.getElementById("newSessionBtn");

let currentSessionId = null;

// ------------------------------------------------------
// UI Helpers
// ------------------------------------------------------
function addMessage(text, sender) {
    const div = document.createElement("div");
    div.className = `msg ${sender}`;
    div.textContent = text;
    chat.appendChild(div);
    chat.scrollTop = chat.scrollHeight;
}

function clearChat() {
    chat.innerHTML = "";
}

function redirectIfUnauthorized(res) {
    if (res.status === 401) {
        window.location.href = "/login";
        return true;
    }
    return false;
}

// ------------------------------------------------------
// API Calls
// ------------------------------------------------------
async function loadSessions() {
    const res = await fetch("/api/ai/sessions", { credentials: "include" });
    if (redirectIfUnauthorized(res)) return;

    const sessions = await res.json();
    sessionSelect.innerHTML = "";

    sessions.forEach(s => {
        const opt = document.createElement("option");
        opt.value = s.id;
        opt.textContent = s.description;
        sessionSelect.appendChild(opt);
    });

    if (sessions.length > 0) {
        if (!currentSessionId) currentSessionId = sessions[0].id;
        sessionSelect.value = currentSessionId;
        loadHistory();
    }
}

async function createSession(description, personaId = "robodad") {
    const res = await fetch("/api/ai/sessions", {
        method: "POST",
        credentials: "include",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ personaId, description })
    });

    if (redirectIfUnauthorized(res)) return null;
    if (!res.ok) {
        alert("Failed to create session");
        return null;
    }

    return res.json();
}

async function loadHistory() {
    clearChat();

    const res = await fetch(`/api/ai/sessions/${currentSessionId}/history`, {
        credentials: "include"
    });

    if (redirectIfUnauthorized(res)) return;

    const messages = await res.json();
    messages.forEach(m => {
        addMessage(m.content, m.role === "user" ? "user" : "bot");
    });
}

async function sendMessage() {
    const text = input.value.trim();
    if (!text) return;

    addMessage(text, "user");
    input.value = "";

    const res = await fetch(`/api/ai/sessions/${currentSessionId}/messages`, {
        method: "POST",
        credentials: "include",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ message: text })
    });

    if (redirectIfUnauthorized(res)) return;

    const data = await res.json();
    addMessage(data.reply, "bot");
}

// ------------------------------------------------------
// Event Listeners
// ------------------------------------------------------
sendBtn.onclick = sendMessage;

input.addEventListener("keydown", e => {
    if (e.key === "Enter") sendMessage();
});

sessionSelect.onchange = () => {
    currentSessionId = sessionSelect.value;
    loadHistory();
};

newSessionBtn.onclick = () => {
    document.getElementById("newChatModal").classList.remove("hidden");
};

document.getElementById("createChatCancel").onclick = () => {
    document.getElementById("newChatModal").classList.add("hidden");
};

document.getElementById("createChatConfirm").onclick = async () => {
    const description = document.getElementById("newChatDescription").value.trim();
    const personaId = document.getElementById("newChatPersona").value;

    if (!description) {
        alert("Please enter a description");
        return;
    }

    const session = await createSession(description, personaId);
    if (!session) return;

    currentSessionId = session.id;

    // Reset modal
    document.getElementById("newChatModal").classList.add("hidden");
    document.getElementById("newChatDescription").value = "";
    document.getElementById("newChatPersona").value = "robodad";

    await loadSessions();
    sessionSelect.value = currentSessionId;
    loadHistory();
};

// ------------------------------------------------------
// Init
// ------------------------------------------------------
loadSessions();
