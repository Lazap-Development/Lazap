<template>
    <div class="parent">
        <div class="leftbar move"></div>
    </div>
</template>

<script>
export default {
    name: "AppOverlay",
    async mounted() {
        Array.from(document.getElementsByClassName("move")).forEach(element => {
            let offsetX, offsetY, isDragging = false;

            element.addEventListener("mousedown", (e) => {
                isDragging = true;
                element.style.position = "absolute";
                offsetX = e.clientX - element.getBoundingClientRect().left;
                offsetY = e.clientY - element.getBoundingClientRect().top;
            });

            document.addEventListener("mousemove", (e) => {
                if (!isDragging) return;

                const x = e.clientX - offsetX;
                const y = e.clientY - offsetY;

                const maxX = window.innerWidth - element.clientWidth;
                const maxY = window.innerHeight - element.clientHeight;

                element.style.left = `${Math.min(Math.max(x, 0), maxX)}px`;
                element.style.top = `${Math.min(Math.max(y, 0), maxY)}px`;
            });


            document.addEventListener("mouseup", () => {
                isDragging = false;
            });
        });

    }
}

</script>

<style>
:root {
    --accentColor: rgb(121, 52, 250);
    --allColorBack: #15161b;
    --allColorPrimary: #18191f;
    --accentColorDark: rgb(121, 52, 250);
    --allColorBackDark: #15161b;
    --allColorPrimaryDark: #18191f;
    --accentColorLight: #FFFF;
    --allColorBackLight: #FFFF;
    --allColorPrimaryLight: #FFFF;
}

* {
    user-select: none !important;
    cursor: default !important;
}

html,
body {
    height: 100%;
    overflow: hidden;
    margin: 0px !important;
    user-select: none;
}

#overlay {
    height: 100%;
}

.parent {
    width: 100%;
    height: 100%;
    background-color: rgba(0, 0, 0, 0.6);
    display: flex;
    justify-content: flex-start;
    align-items: center;
}

.leftbar {
    position: relative;
    background-color: var(--allColorBack);
    width: 150px;
    height: 600px;
    border-radius: 10px;
    left: 50px;
    box-shadow:
        0px 0px 4.7px rgba(0, 0, 0, 0.112),
        0px 0px 11.3px rgba(0, 0, 0, 0.162),
        0px 0px 21.3px rgba(0, 0, 0, 0.2),
        0px 0px 38px rgba(0, 0, 0, 0.238),
        0px 0px 71px rgba(0, 0, 0, 0.288),
        0px 0px 170px rgba(0, 0, 0, 0.4);
    border: 2px solid rgba(128, 128, 128, 0.1);
}

:not(input):not(textarea),
:not(input):not(textarea)::after,
:not(input):not(textarea)::before {
    -webkit-user-select: none;
    user-select: none;
    -webkit-user-drag: none;
    user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;

}
</style>