let currentRpc = "home";

const rpcOptions = {
  home: {
    state: "---",
    details: "Home",
    largeImage: "lazap_icon",
    largeText: "Lazap",
    smallImage: "mushroom",
    smallText: "Lazap",
    id: "home",
  },
  recent: {
    state: "---",
    details: "Recent Games",
    largeImage: "lazap_icon",
    largeText: "Lazap",
    smallImage: "mushroom",
    smallText: "Lazap",
    id: "recent",
  },
  games: {
    state: "Ask them to play with you",
    details: "All Games",
    largeImage: "lazap_icon",
    largeText: "Lazap",
    smallImage: "mushroom",
    smallText: "Lazap",
    id: "games",
  },
  favourites: {
    state: "---",
    details: "Favourite Games",
    largeImage: "lazap_icon",
    largeText: "Lazap",
    smallImage: "mushroom",
    smallText: "Lazap",
    id: "favourites",
  },
  messages: {
    state: "---",
    details: "Messages",
    largeImage: "lazap_icon",
    largeText: "Lazap",
    smallImage: "mushroom",
    smallText: "Lazap",
    id: "messages",
  },
  activity: {
    state: "---",
    details: "Activities",
    largeImage: "lazap_icon",
    largeText: "Lazap",
    smallImage: "mushroom",
    smallText: "Lazap",
    id: "activity",
  },
  friends: {
    state: "---",
    details: "All Friends",
    largeImage: "lazap_icon",
    largeText: "Lazap",
    smallImage: "mushroom",
    smallText: "Lazap",
    id: "friends",
  },
};

const selectOption = (option) => {
  const { home, recent, games, favourites, messages, activity, friends } =
    rpcOptions;
  switch (option) {
    case "home":
      currentRpc = "home";
      return home;
    case "recent":
      currentRpc = "recent";
      return recent;
    case "games":
      currentRpc = "games";
      return games;
    case "favourites":
      currentRpc = "favourites";
      return favourites;
    case "messages":
      currentRpc = "messages";
      return messages;
    case "activity":
      currentRpc = "activity";
      return activity;
    case "friends":
      currentRpc = "friends";
      return friends;
    default:
      throw Error("Invalid Option!");
  }
};

export { selectOption, currentRpc };
