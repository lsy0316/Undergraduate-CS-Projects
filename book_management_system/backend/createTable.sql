CREATE TABLE IF NOT EXISTS books (
  id INT AUTO_INCREMENT PRIMARY KEY,
  title VARCHAR(255) NOT NULL,
  author VARCHAR(255) NOT NULL,
  isbn VARCHAR(13) NOT NULL UNIQUE,
  price DECIMAL(10, 2) NOT NULL,
  category VARCHAR(100) NOT NULL,
  description TEXT,
  published_date DATE NOT NULL,
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

INSERT INTO books (title, author, isbn, price, category, description, published_date) VALUES
('活着', '余华', '9787506358164', 39.00, '文学', '讲述了一个人一生的故事，这是一个历尽世间沧桑和磨难老人的人生感言。', '2012-08-01'),
('三体', '刘慈欣', '9787536692930', 68.00, '科幻', '地球文明向宇宙发出的第一声啼鸣，以太阳为天线，向宇宙深处发出了信号。', '2008-01-01'),
('红楼梦', '曹雪芹', '9787020002200', 98.00, '文学', '中国古典四大名著之一，描写了一个封建贵族家庭由盛而衰的过程。', '1982-03-01'),
('百年孤独', '加西亚·马尔克斯', '9787544244909', 55.00, '文学', '魔幻现实主义文学的代表作，描写了布恩迪亚家族七代人的传奇故事。', '2011-06-01'),
('人类简史', '尤瓦尔·赫拉利', '9787508653883', 68.00, '历史', '从十万年前有生命迹象开始到21世纪资本、科技交织的人类发展史。', '2017-02-01'),
('小王子', '安托万·德·圣-埃克苏佩里', '9787020042509', 32.00, '童话', '一个来自小行星的小王子与飞行员之间的故事，蕴含着深刻的哲理。', '2003-08-01');
