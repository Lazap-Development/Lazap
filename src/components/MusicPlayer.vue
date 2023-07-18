
<template>
    <div class="rightbar">
        <div class="musicPlayer">
            <div class="bkg" id="song-big-cover">
                <div class="bkg-filter">
                    <div class="middle">
                        <div class="lol">
                            <div class="banner" id="song-cover">
                                <img class="subbanner" id="song-subcover" src="../assets/icons/music.png" alt="">
                            </div>
                        </div>
                        <div class="nexto">
                            <p id="song-name">Lazap</p>
                            <p id="artist-name">Lazap Development</p>
                            <div class="family">
                                <div class="button" id="MusicPlayer-backward-btn">
                                    <img src="../assets/svg/music/angle-left-solid.svg" alt="">
                                </div>
                                <div class="button playback" id="MusicPlayer-play-btn">
                                    <img src="../assets/svg/music/play-solid.svg" alt="">
                                </div>
                                <div class="button" id="MusicPlayer-forward-btn">
                                    <img src="../assets/svg/music/angle-right-solid.svg" alt="">
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="slider">
                        <p id="music-progress">0:00</p>
                        <input type="range" id="music-slider" min="1" max="100" value="0">
                        <p id="music-total">0:00</p>
                    </div>
                </div>

            </div>
        </div>
    </div>
</template>

<script>
const invoke = window.__TAURI__.invoke;
const path = window.__TAURI__.path;

export default {
    name: "MusicPlayer",
    async mounted() {
        let LauncherData = JSON.parse(
            await invoke("read_file", {
                filePath: (await path.appDir()) + "LauncherData.json",
            })
        );

        if (LauncherData.enable_spotify) {
            await invoke("spotify_login");
            updatePlayer(JSON.parse(await invoke("spotify_info")));
            setInterval(async () => {
                updatePlayer(JSON.parse(await invoke("spotify_info")));
            }, 1000);
        }

        document.getElementById("MusicPlayer-play-btn").addEventListener("click", async function () {
            let status = await invoke("spotify_toggle_playback")
            if (status == true) {
                let imageElement = document.getElementById("MusicPlayer-play-btn").getElementsByTagName('img')[0];
                imageElement.src = `${require('../assets/svg/music/play-solid.svg')}`
            } else {
                let imageElement = document.getElementById("MusicPlayer-play-btn").getElementsByTagName('img')[0];
                imageElement.src = `${require('../assets/svg/music/pause-solid.svg')}`
            }
        })

        document.getElementById("MusicPlayer-forward-btn").addEventListener("click", async function () {
            await invoke("spotify_forward");
            updatePlayer(JSON.parse(await invoke("spotify_info")));
        })

        document.getElementById("MusicPlayer-backward-btn").addEventListener("click", async function () {
            await invoke("spotify_backward")
            updatePlayer(JSON.parse(await invoke("spotify_info")));
        })

        function updatePlayer(data) {
            if (!data.cover) return;

            document.getElementById("song-name").innerHTML = data.song_name.substring(0, 12);
            if (data.song_name.length > 12) {
                document.getElementById("song-name").innerHTML += "...";
            }
            document.getElementById("artist-name").innerHTML = data.artist_name;
            document.getElementById("song-cover").style.background = `url(${data.cover}`;
            document.getElementById("song-subcover").style.display = "none";

            document.getElementById("song-big-cover").style.background = `url(${data.cover}`
            document.getElementById("song-big-cover").style.backgroundSize = `cover`
            document.getElementById("song-big-cover").style.backgroundRepeat = `no-repeat`
            document.getElementById("song-big-cover").style.backgroundPosition = `center`

            document.getElementById("music-progress").innerHTML = convertMillisecondsToMinutesSeconds(data.progress)
            document.getElementById("music-total").innerHTML = convertMillisecondsToMinutesSeconds(data.duration)

            let status = data.is_playing
            if (status) {
                let imageElement = document.getElementById("MusicPlayer-play-btn").getElementsByTagName('img')[0];
                imageElement.src = `${require('../assets/svg/music/pause-solid.svg')}`

                let time = data.progress
                const percentage = (time / data.duration) * 100;
                time += 1000
                document.getElementById("music-slider").value = percentage;
                let slider = document.getElementById("music-slider");
                slider.style.background = `linear-gradient(to right, var(--accentColor) 0%, var(--accentColor) ${(slider.value - slider.min) / (slider.max - slider.min) * 100}%, white ${(slider.value - slider.min) / (slider.max - slider.min) * 100}%, white 100%)`;
                let event = new Event("change");
                slider.dispatchEvent(event);
            } else {
                let imageElement = document.getElementById("MusicPlayer-play-btn").getElementsByTagName('img')[0];
                imageElement.src = `${require('../assets/svg/music/play-solid.svg')}`
            }

        }

        function convertMillisecondsToMinutesSeconds(milliseconds) {
            var totalSeconds = Math.floor(milliseconds / 1000);
            var minutes = Math.floor(totalSeconds / 60);
            var seconds = totalSeconds % 60;

            var formattedMinutes = minutes < 10 ? "0" + minutes : minutes;
            var formattedSeconds = seconds < 10 ? "0" + seconds : seconds;

            return formattedMinutes + ":" + formattedSeconds;
        }

        // document.getElementById("music-slider").onchange = function () {
        //    this.style.background = `linear-gradient(to right, var(--accentColor) 0%, var(--accentColor) ${(this.value - this.min) / (this.max - this.min) * 100}%, white ${(this.value - this.min) / (this.max - this.min) * 100}%, white 100%)`
        //};
    }
}
</script>

<style>
.rightbar .musicPlayer {
    height: 100%;
    width: 100%;
    padding-bottom: 20px;
    padding-top: 20px;
    padding-left: 10px;
    padding-right: 10px;
    border-radius: 10px;
    display: flex;
    align-items: center;
    justify-content: center;
    box-sizing: border-box
}

.rightbar .musicPlayer .bkg {
    height: 100%;
    width: 100%;
    background: linear-gradient(180deg, rgba(0, 0, 0, 0.4) 0%, var(--accentColor) 90%);
    overflow: hidden;
    border-radius: 15px;
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    box-shadow:
        0px 2.3px 4.6px rgba(0, 0, 0, 0.11),
        0px 18px 37px rgba(0, 0, 0, 0.22);
    padding: 10px;
}

.bkg-filter {
    width: 100%;
    height: 100%;
    backdrop-filter: blur(2px) opacity(90%) brightness(40%);
    display: flex;
    flex-direction: column;
    justify-content: flex-end;
    padding: 10px;
    flex-shrink: 0;
}

.rightbar .musicPlayer .banner {
    width: 80px;
    height: 80px;
    border-radius: 10px;
    background: linear-gradient(360deg, rgba(0, 0, 0, 0.4) 0%, var(--accentColor) 90%);
    background-size: 100% auto !important;
    margin: 0;
    display: flex;
    justify-content: center;
    align-items: center;
}

.subbanner {
    opacity: 0.6;
    width: 40px;
}

.rightbar .musicPlayer .middle {
    display: flex;
    align-items: center;
    margin-left: 5px;
    margin-right: 5px;
    margin-bottom: 13px;
}

.rightbar .musicPlayer .nexto {
    margin-left: 10px;
}

.rightbar .musicPlayer .family {
    display: flex;
    justify-content: flex-start;
    margin-top: 5px;
}

.rightbar .musicPlayer .nexto .button {
    display: flex;
    justify-content: center;
    padding: 3px;
    border-radius: 20px;
    width: 14px;
    height: 14px;
    background-color: #ffffff4a;
    text-decoration: none;
    border: 4px solid var(--accentColor);
    margin: 3px;
    box-shadow:
        0px 5px 10px rgba(0, 0, 0, 0.35),
        0px 40px 80px rgba(0, 0, 0, 0.7);
}

.rightbar .musicPlayer .family .button:hover {
    scale: 1.1;
}

.rightbar .musicPlayer .family .button:active {
    scale: 1;
}

.rightbar .musicPlayer .family .button img {
    width: 15px !important;
}

.rightbar .musicPlayer .nexto p {
    font-family: Nunito-Bold;
    font-size: 14px;
    margin: 0px;
    margin-bottom: -2px !important;
    color: white;
}

.rightbar .musicPlayer .nexto p:first-child {
    font-size: 22px;
    margin-bottom: -5px !important;
    margin: 0px;
}

.rightbar .musicPlayer .slider {
    display: flex;
    flex-direction: row;
}

.rightbar .musicPlayer .slider p {
    margin: 5px;
    font-size: 10px;
    color: white;
}

.rightbar .musicPlayer .slider input {
    width: 100%;
    -webkit-appearance: none;
    height: 3px;
    margin: 10px;
    border-radius: 10px;
    background: white;
    outline: none;
    opacity: 0.8;
    pointer-events: none;
    cursor: default;
}

.rightbar .musicPlayer .slider input::-webkit-slider-thumb {
    -webkit-appearance: none;
    appearance: none;
    width: 20px;
    height: 20px;
    border-radius: 10px;
    background-color: #00000055;
    backdrop-filter: blur(12px);
    border: 4px solid var(--accentColor);
}
</style>