
<template>
  <div class="secondorybox" id="messages">
    <p>Monitor</p>
    <div class="monitor-section">
      <div class="monitor-item">
        <div class="monitor-top">
          <div class="monitor-top-left"></div>
          <div class="monitor-top-right"></div>
        </div>
        <div class="monitor-primary">
          
        </div>
        <div class="monitor-side">
          
        </div>
      </div>
      <div class="monitor-item">

      </div>
    </div>
  </div>
</template>

<script>

</script>

<style>
.monitor-section {
  position: absolute;
  overflow-y: scroll;
  width: 97%;
  height: 81%;
  margin-top: 100px;
  margin-left: 18px;
  display: flex;
  flex-wrap: wrap;
  align-content: flex-start;
  align-items: center;
  justify-content: flex-start;
}

.monitor-item {
  width: 560px;
  height: 280px;
  margin-left: 20px;
  border-radius: 10px;
  display: flex;
  flex-direction: column;
  flex-wrap: wrap;
}

.monitor-item .monitor-top {
  width: 75%;
  height: 70px;
  display: flex;
  align-items: center;
}

.monitor-item .monitor-top .monitor-top-left {
  background-color: var(--allColorBack);
  width: 75px;
  height: 100%;
  border-radius: 20px;
}

.monitor-item .monitor-top .monitor-top-right {
  background-color: var(--allColorBack);
  width: 80%;
  height: 35px;
  margin-left: 20px;
  border-radius: 20px;
}

.monitor-item .monitor-primary {
  margin-top: 10px;
  background-color: var(--allColorBack);
  width: 75%;
  height: 73%;
  border-radius: 20px;
}

.monitor-item .monitor-side {
  background-color: var(--allColorBack);
  height: 500%;
  width: 60px;
  margin-left: 10px; 
  flex: 0 0 calc(100% - 10px);
}
</style>