const chat = document.getElementById("chat");
const input = document.getElementById("messageInput");
const sendBtn = document.getElementById("sendBtn");
const sessionId = document.getElementById("chat-data").dataset.sessionId;

function addMessage(text, sender) {
    const div = document.createElement("div");
    div.className = "msg " + sender;
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

        if (!res.ok) {
            const errText = await res.text();
            addMessage("Error " + res.status + ": " + errText, "bot");
            return;
        }

        const data = await res.json();
        addMessage(data.reply, "bot");
    } catch (err) {
        addMessage("Network error: " + err.message, "bot");
    }
}

sendBtn.onclick = sendMessage;
input.addEventListener("keydown", e => {
    if (e.key === "Enter") sendMessage();
});
