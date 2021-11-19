const si = require('systeminformation');

si.osInfo()
  .then(data => {
    if (data.platform === "linux") {
        console.log("You are on linux!")
    } else if (data.platform === "windows") {
        console.log("You are on windows!")
    }
  })
