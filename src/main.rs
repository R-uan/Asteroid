use ffi::IsKeyDown;
use raylib::prelude::*;
use rand::Rng;

const WINDOW_WIDTH: i32 = 800;
const WINDOW_HEIGHT: i32 = 800;
const PLAYER_SPEED: f32 = 5.0;
const PLAYER_RADIUS: f32 = 10.0;

struct Bullet {
    speed: f32,
    color: Color,
    position: Vector2,
    radius: f32,
}

impl Bullet {
    pub fn new(position: Vector2, color: Color, speed: f32, radius: f32) -> Bullet {
        Bullet {
            position,
            color,
            radius,
            speed,
        }
    }
}

fn main() {
    let (mut rl, thread) = raylib
        ::init()
        .title("Asteroid")
        .height(WINDOW_HEIGHT)
        .width(WINDOW_WIDTH)
        .build();

    let mut player_position: Vector2 = Vector2::new(
        (WINDOW_WIDTH / 2) as f32,
        (WINDOW_HEIGHT / 2) as f32
    );

    let mut rng = rand::thread_rng();
    let mut enemies: Vec<Bullet> = Vec::new();
    let w_width = rl.get_screen_width();
    let w_height = rl.get_screen_height();

    rl.set_target_fps(60);
    while !rl.window_should_close() {
        update_enemies(&mut enemies);
        player_movement(&mut player_position);
        check_screen_boundaries(&mut player_position, &rl);

        let time = rl.get_time();
        let mut d: RaylibDrawHandle = rl.begin_drawing(&thread);

        if (time as i32) % 2 == 0 {
            let random_x = rng.gen_range(0..w_width);
            let bullet_position = Vector2::new(random_x as f32, (0 + 15) as f32);
            let bullet = Bullet::new(bullet_position, Color::RED, PLAYER_SPEED, PLAYER_RADIUS);
            enemies.push(bullet);
        }

        draw_enemies(&enemies, &mut d);

        d.draw_fps(10, 10);
        d.clear_background(Color::BLACK);
        d.draw_circle_v(player_position, PLAYER_RADIUS, Color::RED);
    }
}

fn player_movement(player_position: &mut Vector2) {
    unsafe {
        if IsKeyDown(KeyboardKey::KEY_A as i32) {
            player_position.x -= PLAYER_SPEED;
            println!("x position: {}", player_position.x);
        }
        if IsKeyDown(KeyboardKey::KEY_D as i32) {
            player_position.x += PLAYER_SPEED;
            println!("x position: {}", player_position.x);
        }
        if IsKeyDown(KeyboardKey::KEY_W as i32) {
            player_position.y -= PLAYER_SPEED;
            println!("y position: {}", player_position.y);
        }
        if IsKeyDown(KeyboardKey::KEY_S as i32) {
            player_position.y += PLAYER_SPEED;
            println!("y position: {}", player_position.y);
        }
    }
}

fn check_screen_boundaries(player_position: &mut Vector2, rl: &RaylibHandle) {
    let w_height = rl.get_screen_height();
    let w_width = rl.get_screen_width();

    if player_position.x <= PLAYER_RADIUS {
        player_position.x = PLAYER_RADIUS;
    }

    if player_position.x >= (w_width as f32) - PLAYER_RADIUS {
        player_position.x = (w_width as f32) - PLAYER_RADIUS;
    }

    if player_position.y <= PLAYER_RADIUS {
        player_position.y = PLAYER_RADIUS;
    }

    if player_position.y >= (w_height as f32) - PLAYER_RADIUS {
        player_position.y = (w_height as f32) - PLAYER_RADIUS;
    }
}

fn draw_enemies(enemies: &Vec<Bullet>, rld: &mut RaylibDrawHandle) {
    for enemy in enemies {
        rld.draw_circle_v(enemy.position, enemy.radius, enemy.color);
    }
}

fn update_enemies(enemies: &mut Vec<Bullet>) {
    for enemy in enemies {
        enemy.position.y += enemy.speed;
    }
}
