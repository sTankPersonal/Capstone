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

async function sendMessage() {
    const text = input.value.trim();
    if (!text) return;

    addMessage(text, "user");
    input.value = "";

    try {
        const res = await fetch(`/user/chats/${sessionId}/messages`, {
            method: "POST",
            headers: { "Content-Type": "application/x-www-form-urlencoded" },
            body: new URLSearchParams({ message: text })
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
