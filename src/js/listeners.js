const marker = document.querySelector("#indicator");
const items = document.querySelectorAll(".side-tab");



items.forEach(link => link.addEventListener("click", e => {
    marker.style.top = "220px";
    marker.style.height = "0px";
    indicator(e.target)
}));

function indicator(item) {
    marker.style.top = item.offsetTop + "px";
    marker.style.height = "30px";
}

window.onkeydown = function (evt) {
    if ((evt.code == "Minus" || evt.code == "Equal") && (evt.ctrlKey || evt.metaKey)) {
        evt.preventDefault()
    }
}