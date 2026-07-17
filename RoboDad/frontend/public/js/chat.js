var main = document.querySelector('.chats-main');
var userId = main.dataset.userId;
var sessionId = main.dataset.sessionId;
var chat = document.getElementById('chat');
var input = document.getElementById('messageInput');


input.addEventListener('input', () => {
    // If empty, collapse to default height
    if (input.value.trim() === '') {
        input.style.height = 'auto';
        return;
    }

    // Otherwise grow up to a max height
    const maxHeight = 160; // ~10rem
    input.style.height = 'auto';
    input.style.height = Math.min(input.scrollHeight, maxHeight) + 'px';
});



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
var isWaiting = false;

function sendMessage() {
    if (isWaiting) return;

    var text = input.value.trim();
    if (!text) return;

    isWaiting = true;
    input.disabled = true;
    document.getElementById('sendBtn').disabled = true;

    addMessage(text, 'user');
    input.value = '';
    input.style.height = 'auto';
    document.getElementById('charCounter').textContent = `0 / 1200`;


    // Show typing bubble immediately
    var typingBubble = showTypingBubble();

    var params = new URLSearchParams({ message: text });

    fetch('/user/chats/' + sessionId + '/messages', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: params.toString()
    })
        .then(function (res) {
            if (res.status === 401) { window.location.href = '/login'; return null; }
            if (!res.ok) {
                typingBubble.remove();
                addMessage('Error sending message. Please try again.', 'assistant');
                return null;
            }
            return res.json();
        })
        .then(function (data) {
            if (data) {
                typingBubble.remove();
                animateAssistantMessage(data.reply);
            }
        })
        .catch(function () {
            typingBubble.remove();
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


function showTypingBubble() {
    const bubble = document.createElement('div');
    bubble.className = 'msg assistant typing-bubble';
    bubble.textContent = 'Organizing my thoughts...';
    chat.appendChild(bubble);
    chat.scrollTop = chat.scrollHeight;
    return bubble;
}

const MAX_MESSAGE_LENGTH = 1200;

input.addEventListener('input', () => {
    if (input.value.length > MAX_MESSAGE_LENGTH) {
        input.value = input.value.slice(0, MAX_MESSAGE_LENGTH);
    }

    // Auto-resize logic
    if (input.value.trim() === '') {
        input.style.height = 'auto';
        return;
    }

    const maxHeight = 160;
    input.style.height = 'auto';
    input.style.height = Math.min(input.scrollHeight, maxHeight) + 'px';
});
const counter = document.getElementById('charCounter');

input.addEventListener('input', () => {
    if (input.value.length > MAX_MESSAGE_LENGTH) {
        input.value = input.value.slice(0, MAX_MESSAGE_LENGTH);
    }

    counter.textContent = `${input.value.length} / ${MAX_MESSAGE_LENGTH}`;
});



function animateAssistantMessage(fullText, chunkSize = 30, delay = 70) {
    // Show typing bubble first
    const typingBubble = showTypingBubble();

    // After a short delay, replace typing bubble with real message
    setTimeout(() => {
        typingBubble.remove();

        const div = document.createElement('div');
        div.className = 'msg assistant';
        chat.appendChild(div);

        let index = 0;

        function step() {
            const partial = fullText.slice(0, index);
            const html = renderMarkdown(partial);
            if (html !== null) div.innerHTML = html;
            else div.textContent = partial;

            chat.scrollTop = chat.scrollHeight;

            index += chunkSize;

            if (index <= fullText.length) {
                setTimeout(step, delay);
            } else {
                // Animation finished — unlock input
                isWaiting = false;
                input.disabled = false;
                document.getElementById('sendBtn').disabled = false;
                input.focus();
            }
        }

        step();
    }, 300); // typing bubble visible for 300ms
}
