<template>
  <div class="secondorybox" id="monitor">
    <div class="secondary-top">
      <p>Monitor</p>
    </div>
    <div class="monitor-section">
      <div class="monitor-parent">
        <div class="monitor-item">
          <div class="monitor-top">
            <div class="monitor-top-left"></div>
            <div class="monitor-top-right"></div>
          </div>
          <div class="monitor-primary"></div>
        </div>
        <div class="monitor-item-besides">
          <div class="monitor-side"></div>
        </div>
      </div>
      <div class="monitor-parent">
        <div class="monitor-item">
          <div class="monitor-top">
            <div class="monitor-top-left"></div>
            <div class="monitor-top-right"></div>
          </div>
          <div class="monitor-primary"></div>
        </div>
        <div class="monitor-item-besides">
          <div class="monitor-side"></div>
        </div>
      </div>
      <div class="monitor-parent">
        <div class="monitor-item">
          <div class="monitor-top">
            <div class="monitor-top-left"></div>
            <div class="monitor-top-right"></div>
          </div>
          <div class="monitor-primary"></div>
        </div>
        <div class="monitor-item-besides">
          <div class="monitor-side"></div>
        </div>
      </div>
      <div class="monitor-parent">
        <div class="monitor-item">
          <div class="monitor-top">
            <div class="monitor-top-left"></div>
            <div class="monitor-top-right"></div>
          </div>
          <div class="monitor-primary"></div>
        </div>
        <div class="monitor-item-besides">
          <div class="monitor-side"></div>
        </div>
      </div>
    </div>
  </div>
</template>

<script></script>

<style>
.monitor-section {
  position: absolute;
  overflow-y: scroll;
  width: 97%;
  height: 72%;
  margin-top: 100px;
  margin-left: 18px;
  display: flex;
  flex-wrap: wrap;
  align-content: flex-start;
  align-items: center;
  justify-content: flex-start;
  color: white;
}

.monitor-parent {
  display: flex;
  margin-bottom: 40px;
}

.monitor-item {
  width: 420px;
  height: 320px;
  margin-left: 20px;
  border-radius: 10px;
  display: flex;
  flex-direction: column;
}

.monitor-item .monitor-top {
  width: 100%;
  height: 70px;
  display: flex;
  align-items: center;
}

.monitor-item .monitor-top .monitor-top-left {
  background-color: rgba(var(--all-color-front), 1);
  width: 80px;
  height: 94%;
  border-radius: 20px;
  box-shadow: 0.5px 1px 1.3px rgba(0, 0, 0, 0.5),
    4px 8px 10px rgba(0, 0, 0, 0.25);
  display: flex;
  justify-content: center;
  align-items: center;
}

.monitor-item .monitor-top .monitor-top-right {
  background-color: rgba(var(--all-color-front), 1);
  width: 100%;
  height: 35px;
  margin-left: 20px;
  border-radius: 20px;
  box-shadow: 0.5px 1px 1.3px rgba(0, 0, 0, 0.5),
    4px 8px 10px rgba(0, 0, 0, 0.25);
}

.monitor-item .monitor-primary {
  margin-top: 10px;
  background-color: rgba(var(--all-color-front), 1);
  width: 100%;
  height: 73%;
  border-radius: 20px;
  box-shadow: 0.5px 1px 1.3px rgba(0, 0, 0, 0.5),
    4px 8px 10px rgba(0, 0, 0, 0.25);
}

.monitor-item-besides {
  width: 100px;
  height: 320px;
  border-radius: 10px;
  display: flex;
  flex-direction: column;
  margin-right: 40px;
}

.monitor-item-besides .monitor-side {
  background-color: rgba(var(--all-color-front), 1);
  height: 100%;
  width: 100px;
  margin-left: 20px;
  border-radius: 20px;
  box-shadow: 0px 1px 1.3px rgba(0, 0, 0, 0.5), 0px 8px 10px rgba(0, 0, 0, 0.25);
}
</style>
