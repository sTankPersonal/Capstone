var main = document.querySelector('.chats-main');
var userId = main.dataset.userId;
var sessionId = main.dataset.sessionId;
var chat = document.getElementById('chat');
var input = document.getElementById('messageInput');

// Assistant replies come back as Markdown; sanitize the rendered HTML since
// it goes into innerHTML. Falls back to plain text if the CDN scripts fail.
function renderMarkdown(text) {
    if (!window.marked || !window.DOMPurify) return null;
    return DOMPurify.sanitize(marked.parse(text, { breaks: true }));
}

function setMessageContent(el, text, role) {
    var html = role === 'assistant' ? renderMarkdown(text) : null;
    if (html !== null) el.innerHTML = html;
    else el.textContent = text;
}

document.querySelectorAll('#chat .msg').forEach(function (msg) {
    var role = msg.dataset.sender === 'user' ? 'user' : 'assistant';
    msg.classList.add(role);
    setMessageContent(msg, msg.textContent, role);
});

chat.scrollTop = chat.scrollHeight;

function addMessage(text, role) {
    var div = document.createElement('div');
    div.className = 'msg ' + role;
    setMessageContent(div, text, role);
    chat.appendChild(div);
    chat.scrollTop = chat.scrollHeight;
}

function sendMessage() {
    var text = input.value.trim();
    if (!text) return;

    addMessage(text, 'user');
    input.value = '';

    var params = new URLSearchParams({ message: text });
    fetch('/user/chats/' + sessionId + '/messages', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: params.toString()
    }).then(function (res) {
        if (res.status === 401) { window.location.href = '/login'; return null; }
        if (!res.ok) { addMessage('Error sending message. Please try again.', 'assistant'); return null; }
        return res.json();
    }).then(function (data) {
        if (data) addMessage(data.reply, 'assistant');
    }).catch(function () {
        addMessage('Error sending message. Please try again.', 'assistant');
    });
}

document.getElementById('sendBtn').addEventListener('click', sendMessage);
input.addEventListener('keydown', function (e) {
    if (e.key === 'Enter') sendMessage();
});

var activePath = '/user/chats/' + sessionId + '/messages';
document.querySelectorAll('.chat-list-item').forEach(function (a) {
    if (a.getAttribute('href') === activePath) a.classList.add('active');
});
