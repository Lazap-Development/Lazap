let currentRpc = "home";

const rpcOptions = {
    "home": {
        "state": "__",
        "details": "On main screen",
        "largeImage": "lazap_icon",
        "largeText": "Lazap",
        "smallImage": "controller_icon",
        "smallText": "Lazap",
        "id": "home"
    },
    "recent": {
        "state": "__",
        "details": "Watching recent games",
        "largeImage": "lazap_icon",
        "largeText": "Lazap",
        "smallImage": "controller_icon",
        "smallText": "Lazap",
        "id": "recent"
    },
    "games": {
        "state": "ask them to play with you",
        "details": "Picking games",
        "largeImage": "lazap_icon",
        "largeText": "Lazap",
        "smallImage": "controller_icon",
        "smallText": "Lazap",
        "id": "games"
    },
    "favourites": {
        "state": "__",
        "details": "Watching favourite games",
        "largeImage": "lazap_icon",
        "largeText": "Lazap",
        "smallImage": "controller_icon",
        "smallText": "Lazap",
        "id": "favourites"
    },
    "messages": {
        "state": "__",
        "details": "Watching messages",
        "largeImage": "lazap_icon",
        "largeText": "Lazap",
        "smallImage": "controller_icon",
        "smallText": "Lazap",
        "id": "messages"
    },
    "activity": {
        "state": "don't think so",
        "details": "Are activities even a thing?",
        "largeImage": "lazap_icon",
        "largeText": "Lazap",
        "smallImage": "controller_icon",
        "smallText": "Lazap",
        "id": "activity"
    },
    "friends": {
        "state": "bet you don't have any",
        "details": "Scrolling through friends list",
        "largeImage": "lazap_icon",
        "largeText": "Lazap",
        "smallImage": "controller_icon",
        "smallText": "Lazap",
        "id": "friends"
    }
}

const selOption = option => {
    const { home, recent, games, favourites, messages, activity, friends } = rpcOptions
    switch (option) {
        case "home":
            currentRpc = "home"
            return home;
        case "recent":
            currentRpc = "recent"
            return recent;
        case "games":
            currentRpc = "games"
            return games;
        case "favourites":
            currentRpc = "favourites"
            return favourites;
        case "messages":
            currentRpc = "messages"
            return messages;
        case "activity":
            currentRpc = "activity"
            return activity;
        case "friends":
            currentRpc = "friends"
            return friends;
        default:
            throw Error("Invalid Option!");
    }
}

export {
    selOption,
    currentRpc
};