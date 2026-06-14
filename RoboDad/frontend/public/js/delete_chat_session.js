var path = window.location.pathname.replace('/delete', '/messages');
document.querySelectorAll('.chat-list-item').forEach(function (a) {
    if (a.getAttribute('href') === path) a.classList.add('active');
});
