const delay = (ms) => new Promise((resolve) => setTimeout(resolve, ms));


var KromBlast = new class {
    constructor() {
        this.krom_id = 'KromBlastTest55698';
        this.init_load = false;
        this.is_in_krom = false;
        this.id_loaded = false;
        this.api = null;
        this.plugins = {};
        this.event = {
            "initialized": () => { },
            "not_in_krom": () => { },
            "id_refused": () => { },
        }
        window.addEventListener('pywebviewready', this.init.bind(this));
        setTimeout(
            () => { if (!this.init_load) { this.init() } },
            1000
        )
    }

    set_event(event_name, callback) {
        this.event[event_name] = callback;
    }

    set_id(id) {
        this.krom_id = id;
    }

    async decode_plugin_data(entry, api_data) {
        for (var plugin_name of Object.keys(api_data)) {
            entry[plugin_name] = new class { }();
            for (const [key, value] of Object.entries(api_data[plugin_name])) {
                switch (value[0]) {
                    case 'function':
                        entry[plugin_name][key] = async (...args) => {
                            return await this.api.call_plugin_function(plugin_name, key, ...args);
                        };
                        break;
                    case 'object':
                        break;
                    /*
                    entry[plugin_name][key] = new class{}();
                    await this.decode_plugin_data(entry[plugin_name][key], value[1]);
                    break;*/
                    case 'variable':
                        entry[plugin_name][key] = value[1];
                        break;
                    default:
                        break;
                }
            }
        }
    }

    async init() {
        this.init_load = true;
        if (
            typeof window.pywebview !== 'undefined' &&
            typeof window.pywebview.api !== 'undefined' &&
            (await window.pywebview.api.is_kromblast_running())
        ) {
            this.api = window.pywebview.api;
            if (await this.api.is_debug_mode()) {
                console.log("KromBlast is running in debug mode");
            } else {
                console.log = (...args) => { window.pywebview.api.log(...args) };
            }
            this.is_in_krom = true;

            var api_data = await this.api.get_plugins_data(this.krom_id);
            if (api_data == false) {
                this.event.id_refused();
                return;                
            }
            await this.decode_plugin_data(this, api_data);

            console.log('KromBlast initialized');

            this.event["initialized"]();

        } else {
            console.log("KromBlast is not running");
            this.is_in_krom = false;
            this.event.not_in_krom();
        }
    };
}()