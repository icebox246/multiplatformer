(async () => {
    const canvas = document.querySelector('#canvas');
    const fpsDisplay = document.querySelector('#fps');
    const ctx = canvas.getContext('2d');

    const utf8decoder = new TextDecoder();

    let memoryView;

    const textures = [];

    const imports = {
        env: {
            platform_rect(x, y, w, h, color) {
                color = (new Uint32Array([color]))[0]
                ctx.fillStyle = '#' + color.toString(16);
                x = Math.round(x);
                y = Math.round(y);
                h = Math.round(h);
                w = Math.round(w);
                ctx.fillRect(x, y, w, h);
            },
            platform_print(ptr) {
                const buff = [];
                let i = ptr;
                while (memoryView[i]) {
                    buff.push(memoryView[i]);
                    i++;
                }
                console.log(utf8decoder.decode(new Uint8Array(buff)));
            },
            platform_frand() {
                return Math.random();
            },
            platform_load_texture(ptr) {
                const buff = [];
                let i = ptr;
                while (memoryView[i]) {
                    buff.push(memoryView[i]);
                    i++;
                }
                const assetName = utf8decoder.decode(new Uint8Array(buff));
                const url = `../assets/${assetName}`;

                console.log('[INFO] Loading', url);

                const img = new Image();
                img.src = url;
                const texId = textures.length;
                textures[texId] = {tex: img, rect: {x: 0, y: 0, w: 0, h: 0}};
                (new Promise((resolve, reject) => {
                    const timeout =
                        setTimeout(() => reject('failed to load asset'), 3000);
                    img.onload = () => {
                        clearTimeout(timeout);
                        textures[texId].rect.w = img.width;
                        textures[texId].rect.h = img.height;
                        resolve(true);
                    };
                })).catch(console.error);
            }
        }
    }

    const module =
        await WebAssembly.instantiateStreaming(fetch('./game.wasm'), imports);

    memoryView = new Uint8Array(module.instance.exports.memory.buffer);

    let lastTime = null;
    let frames = 0;
    let mousePos = {x: 0, y: 0};
    module.instance.exports.game_init();
    const loop = (timeNow) => {
        const dt = (timeNow - lastTime) * 0.001;
        lastTime = timeNow;
        module.instance.exports.game_update(dt);

        ctx.fillStyle = 'black';
        ctx.fillRect(0, 0, canvas.clientWidth, canvas.clientHeight);
        module.instance.exports.game_render();

        frames++;

        window.requestAnimationFrame(loop);
    };
    const first = (timestamp) => {
        lastTime = timestamp;
        window.requestAnimationFrame(loop)
    };

    window.requestAnimationFrame(first);

    module.instance.exports.game_render();

    setInterval(() => {
        fpsDisplay.innerText = `FPS: ${frames}`;
        frames = 0;
    }, 1000);

    window.addEventListener('keydown', (e) => {
        module.instance.exports.game_key_down(e.keyCode);
    })
    window.addEventListener('keyup', (e) => {
        module.instance.exports.game_key_up(e.keyCode);
    })
    canvas.addEventListener('mousemove', (e) => {
        module.instance.exports.game_mouse_move(e.offsetX, e.offsetY);
    })
})()
