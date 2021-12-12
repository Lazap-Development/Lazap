const axios = require("axios")

async function makeGetRequest() {

  let res = await axios.get("https://store.steampowered.com/api/appdetails?appids=1708091");

  let data = res.data[1708091].data.header_image;
  return data
}

makeGetRequest()