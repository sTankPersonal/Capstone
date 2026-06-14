var main = document.querySelector('.chats-main');
var userId = main.dataset.userId;
var sessionId = main.dataset.sessionId;
var chat = document.getElementById('chat');
var input = document.getElementById('messageInput');

document.querySelectorAll('#chat .msg').forEach(function (msg) {
    msg.classList.add(msg.dataset.sender === 'user' ? 'user' : 'assistant');
});

chat.scrollTop = chat.scrollHeight;

function addMessage(text, role) {
    var div = document.createElement('div');
    div.className = 'msg ' + role;
    div.textContent = text;
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
