import type { Book } from '../types/book';

export const mockBooks: Book[] = [
  {
    id: 1,
    title: '活着',
    author: '余华',
    isbn: '9787506358164',
    price: 39.00,
    category: '文学',
    description: '讲述了一个人一生的故事，这是一个历尽世间沧桑和磨难老人的人生感言。',
    published_date: '2012-08-01'
  },
  {
    id: 2,
    title: '三体',
    author: '刘慈欣',
    isbn: '9787536692930',
    price: 68.00,
    category: '科幻',
    description: '地球文明向宇宙发出的第一声啼鸣，以太阳为天线，向宇宙深处发出了信号。',
    published_date: '2008-01-01'
  },
  {
    id: 3,
    title: '红楼梦',
    author: '曹雪芹',
    isbn: '9787020002200',
    price: 98.00,
    category: '文学',
    description: '中国古典四大名著之一，描写了一个封建贵族家庭由盛而衰的过程。',
    published_date: '1982-03-01'
  },
  {
    id: 4,
    title: '百年孤独',
    author: '加西亚·马尔克斯',
    isbn: '9787544244909',
    price: 55.00,
    category: '文学',
    description: '魔幻现实主义文学的代表作，描写了布恩迪亚家族七代人的传奇故事。',
    published_date: '2011-06-01'
  },
  {
    id: 5,
    title: '人类简史',
    author: '尤瓦尔·赫拉利',
    isbn: '9787508653883',
    price: 68.00,
    category: '历史',
    description: '从十万年前有生命迹象开始到21世纪资本、科技交织的人类发展史。',
    published_date: '2017-02-01'
  },
  {
    id: 6,
    title: '小王子',
    author: '安托万·德·圣-埃克苏佩里',
    isbn: '9787020042509',
    price: 32.00,
    category: '童话',
    description: '一个来自小行星的小王子与飞行员之间的故事，蕴含着深刻的哲理。',
    published_date: '2003-08-01'
  }
];
