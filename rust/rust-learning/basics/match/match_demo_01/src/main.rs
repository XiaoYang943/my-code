use std::env;
use std::fs;
use std::num::ParseIntError;
/// 配置加载:
/// 从环境变量读取配置
/// 配置可能缺失
/// 配置可能格式错误
/// 校验业务规则
/// 错误要统一、可传播

// =======================
// 1. 定义应用级错误
// =======================

#[derive(Debug)]
enum AppError {
    MissingEnv(String),
    InvalidNumber(String),
    InvalidValue(String),
    IoError(String),
}

// =======================
// 2. 应用配置结构
// =======================

#[derive(Debug)]
struct AppConfig {
    port: u16,
    workers: usize,
}

// =======================
// 3. 工具函数（每一步都可能失败）
// =======================

// 读取环境变量（可能不存在）
fn get_env(key: &str) -> Result<String, AppError> {
    env::var(key)
        .map_err(|_| AppError::MissingEnv(key.to_string()))
}

// 解析 u16
fn parse_u16(s: String, field: &str) -> Result<u16, AppError> {
    s.parse::<u16>()
        .map_err(|_| AppError::InvalidNumber(field.to_string()))
}

// 解析 usize
fn parse_usize(s: String, field: &str) -> Result<usize, AppError> {
    s.parse::<usize>()
        .map_err(|_| AppError::InvalidNumber(field.to_string()))
}

// 业务校验
fn validate_port(port: u16) -> Result<u16, AppError> {
    if port == 0 {
        Err(AppError::InvalidValue("port must be > 0".into()))
    } else {
        Ok(port)
    }
}

fn validate_workers(workers: usize) -> Result<usize, AppError> {
    if workers == 0 {
        Err(AppError::InvalidValue("workers must be > 0".into()))
    } else {
        Ok(workers)
    }
}

// =======================
// 4. 核心流程：加载配置
// =======================

fn load_config() -> Result<AppConfig, AppError> {
    let port = get_env("APP_PORT")
        .and_then(|s| parse_u16(s, "APP_PORT"))
        .and_then(validate_port)?;

    let workers = get_env("APP_WORKERS")
        .and_then(|s| parse_usize(s, "APP_WORKERS"))
        .and_then(validate_workers)?;

    Ok(AppConfig { port, workers })
}

// =======================
// 5. main：应用入口
// =======================

fn main() {
    match load_config() {
        Ok(config) => {
            println!("Config loaded: {:?}", config);
            println!("Server running on port {}", config.port);
        }
        Err(e) => {
            eprintln!("Failed to start application: {:?}", e);
        }
    }
}
