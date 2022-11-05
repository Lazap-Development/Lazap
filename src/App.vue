<template>
	<KeepAlive :max="2">
		<component :is="currentComponent" :change-component="changeComponent" />
	</KeepAlive>
</template>
<script>
import Main from './Main.vue';
import Login from './Login.vue';

export default {
	data() {
		return {
			currentComponent: null,
			components: {
				Login, Main,
			},
		};
	},
	async created() {
		const os = window.__TAURI__.os;
		console.info(`OS: ${await os.platform()} | Arch: ${await os.arch()}`);

		if (!this.currentComponent) {
			this.changeComponent('Login');
		}
	},
	methods: {
		min_window() {
			window.__TAURI__.window.appWindow.minimize()
		},
		max_window() {
			window.__TAURI__.window.appWindow.toggleMaximize()
		},
		close_window() {
			window.__TAURI__.window.appWindow.close()
		},
		changeComponent(component) {
			this.currentComponent = this.components[component];
			this.currentComponent.load();
		},
	},
}
</script>