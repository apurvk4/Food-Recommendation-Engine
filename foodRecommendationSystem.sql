CREATE DATABASE IF NOT EXISTS FoodRecommendationSystem;

USE FoodRecommendationSystem;

CREATE TABLE `FoodItemType`(
    `foodItemTypeId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `foodItemType` VARCHAR(255) NOT NULL
);

INSERT INTO `FoodItemType`(`foodItemType`) VALUES('Breakfast'), ('Lunch'), ('Dinner');

CREATE TABLE `FoodItem`(
    `foodItemId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `itemName` VARCHAR(255) NOT NULL,
    `price` DOUBLE NOT NULL,
    `availabilityStatus` TINYINT(1) NOT NULL DEFAULT '1',
    `foodItemTypeId` BIGINT UNSIGNED NOT NULL REFERENCES `FoodItemType`(`foodItemTypeId`) ON DELETE CASCADE
);

CREATE TABLE `Role`(
    `roleId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `roleName` VARCHAR(255) NOT NULL
);

INSERT INTO `Role`(`roleName`) VALUES('Admin'), ('Employee'), ('Chef');

CREATE TABLE `User`(
    `userId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `name` VARCHAR(255) NOT NULL,
    `password` VARCHAR(255) NOT NULL,
    `roleId` BIGINT UNSIGNED NOT NULL REFERENCES `Role`(`roleId`) ON DELETE CASCADE,
    `lastNotificationId` BIGINT UNSIGNED NULL DEFAULT NULL REFERENCES `Notification`(`notificationId`) ON DELETE SET NULL
);

INSERT INTO `User`(`name`, `password`, `roleId`) VALUES('admin', 'admin', 1), ('employee', 'employee', 2), ('chef', 'chef', 3);


CREATE TABLE `Login`(
    `loginId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `userId` BIGINT UNSIGNED NOT NULL,
    `loginTime` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (`userId`) REFERENCES `User`(`userId`) ON DELETE CASCADE
);

CREATE TABLE `Notification`(
    `notificationId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `message` TEXT NOT NULL,
    `date` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE `MenuItem`(
    `menuItemId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `menuId` BIGINT UNSIGNED NOT NULL REFERENCES `Menu`(`menuId`) ON DELETE CASCADE,
    `foodItemId` BIGINT UNSIGNED NOT NULL REFERENCES `FoodItem`(`foodItemId`) ON DELETE CASCADE,
    `quantity` INT NOT NULL DEFAULT 1
);

ALTER TABLE
    `MenuItem` ADD INDEX `fooditemsinmenu_menuid_index`(`menuId`);

ALTER TABLE
    `MenuItem` ADD INDEX `fooditemsinmenu_fooditemid_index`(`foodItemId`);

CREATE TABLE `MenuType`(
    `menuTypeId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `menuType` VARCHAR(255) NOT NULL
);

INSERT INTO `MenuType`(`menuType`) VALUES('Breakfast'), ('Lunch'), ('Dinner'),('Breakfast Servey'), ('Lunch Servey'), ('Dinner Servey');

CREATE TABLE `Menu`(
    `menuId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `menuName` VARCHAR(255) NOT NULL,
    `menuTypeId` BIGINT UNSIGNED NULL REFERENCES `MenuType`(`menuTypeId`) ON DELETE SET NULL
);

CREATE TABLE `ScheduledMenu` (
    `scheduledMenuId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `menuId` BIGINT UNSIGNED NOT NULL,
    `date` DATETIME NOT NULL,
    `isSurveyMenu` TINYINT(1) NOT NULL DEFAULT 1,
    FOREIGN KEY (`menuId`) REFERENCES `Menu`(`menuId`) ON DELETE CASCADE
);


ALTER TABLE
    `ScheduledMenu` ADD INDEX `scheduledmenu_date_index`(`date`);

CREATE TABLE `Review`(
    `reviewId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `userId` BIGINT UNSIGNED NULL REFERENCES `User`(`userId`) ON DELETE SET NULL,
    `foodItemId` BIGINT UNSIGNED NOT NULL REFERENCES `FoodItem`(`foodItemId`) ON DELETE CASCADE,
    `rating` INT NOT NULL,
    `comment` TEXT NOT NULL,
    `date` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
);

ALTER TABLE
    `Review` ADD INDEX `reviews_userid_index`(`userId`);

ALTER TABLE 
    `Review` ADD INDEX `reviews_fooditemid_index`(`foodItemId`);

CREATE TABLE `Feedback`(
    `feedbackId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `userId` BIGINT UNSIGNED NULL REFERENCES `User`(`userId`) ON DELETE SET NULL,
    `menuId` BIGINT UNSIGNED NOT NULL REFERENCES `Menu`(`menuId`) ON DELETE CASCADE,
    `foodItemId` BIGINT UNSIGNED NOT NULL REFERENCES `FoodItem`(`foodItemId`) ON DELETE CASCADE,
    `preference` BOOLEAN NOT NULL
);



DELIMITER $$

CREATE TRIGGER trg_add_user_to_last_notification
AFTER INSERT ON User
FOR EACH ROW
BEGIN
    INSERT INTO UserLastNotification (userId, lastNotificationId)
    VALUES (NEW.userId, NULL);
END $$


CREATE TRIGGER trg_restrict_userResponse_insert
BEFORE INSERT ON Feedback
FOR EACH ROW
BEGIN
    DECLARE menuTypeExists INT;

    SELECT COUNT(*) INTO menuTypeExists 
    FROM Menu M
    JOIN MenuType MT ON M.menuTypeId = MT.menuTypeId
    WHERE M.menuId = NEW.menuId
        AND MT.menuType IN ('Breakfast Servey', 'Lunch Servey', 'Dinner Servey');

    IF menuTypeExists = 0 THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'User responses are not allowed for this menu';
    END IF;
END $$

CREATE TRIGGER trg_remove_null_feedback
AFTER UPDATE ON Review
FOR EACH ROW
BEGIN
    IF NEW.userId IS NULL AND NEW.foodItemId IS NULL THEN
        DELETE FROM Review WHERE userId IS NULL AND foodItemId IS NULL;
    END IF;
END $$

CREATE TRIGGER trg_create_menu_notification
AFTER INSERT ON ScheduledMenu
FOR EACH ROW
BEGIN
    DECLARE notification_message TEXT;
    SET notification_message = CONCAT('New menu added on ', NEW.date);
    INSERT INTO Notification (message, date) VALUES (notification_message, NOW());
END $$

CREATE TRIGGER trg_create_fooditem_notification
AFTER UPDATE ON FoodItem
FOR EACH ROW
BEGIN
    DECLARE notification_message TEXT;
    SET notification_message = CONCAT('Food item "', OLD.itemName, '" (ID: ', OLD.foodItemId, ') updated. New price: ', NEW.price, ', Availability status: ', NEW.availabilityStatus);
    INSERT INTO Notification (message, date) VALUES (notification_message, NOW());
END $$

CREATE TRIGGER trg_create_fooditem_insert_notification
AFTER INSERT ON FoodItem
FOR EACH ROW
BEGIN
    DECLARE notification_message TEXT;
    SET notification_message = CONCAT('New food item "', NEW.itemName, '" (ID: ', NEW.foodItemId, ') added. Price: ', NEW.price, ', Availability status: ', NEW.availabilityStatus);
    INSERT INTO Notification (message, date) VALUES (notification_message, NOW());
END $$

CREATE TRIGGER trg_create_fooditem_delete_notification
AFTER DELETE ON FoodItem
FOR EACH ROW
BEGIN
    DECLARE notification_message TEXT;
    SET notification_message = CONCAT('Food item "', OLD.itemName, '" (ID: ', OLD.foodItemId, ') deleted');
    INSERT INTO Notification (message, date) VALUES (notification_message, NOW());
END $$

SET GLOBAL event_scheduler = ON $$

CREATE EVENT delete_old_notifications
ON SCHEDULE EVERY 1 DAY
STARTS CURRENT_TIMESTAMP
DO
BEGIN
    DELETE FROM Notification WHERE date < NOW() - INTERVAL 1 WEEK;
END $$

CREATE EVENT remove_expired_sessions
ON SCHEDULE EVERY 1 DAY
STARTS CURRENT_TIMESTAMP
DO
BEGIN
    DELETE FROM `Session` WHERE `validTill` < NOW();
END $$


DELIMITER ;	

INSERT INTO `FoodItem`(`itemName`, `price`, `availabilityStatus`, `foodItemTypeId`) VALUES
('Idli', 30.00, 1, 1), -- Breakfast
('Dosa', 50.00, 1, 1), -- Breakfast
('Upma', 40.00, 1, 1), -- Breakfast
('Poha', 35.00, 1, 1), -- Breakfast
('Paratha', 45.00, 1, 1), -- Breakfast
('Paneer Butter Masala', 150.00, 1, 2), -- Lunch
('Chicken Biryani', 180.00, 1, 2), -- Lunch
('Vegetable Pulao', 120.00, 1, 2), -- Lunch
('Dal Tadka', 80.00, 1, 2), -- Lunch
('Rajma Chawal', 100.00, 1, 2), -- Lunch
('Palak Paneer', 140.00, 1, 2), -- Lunch
('Masala Dosa', 60.00, 1, 1), -- Breakfast
('Aloo Gobi', 90.00, 1, 2), -- Lunch
('Samosa', 20.00, 1, 1), -- Breakfast
('Butter Chicken', 200.00, 1, 3), -- Dinner
('Rogan Josh', 220.00, 1, 3), -- Dinner
('Chole Bhature', 80.00, 1, 2), -- Lunch
('Paneer Tikka', 130.00, 1, 3), -- Dinner
('Fish Curry', 160.00, 1, 3), -- Dinner
('Gulab Jamun', 50.00, 1, 3); -- Dinner

INSERT INTO `Review` (`userId`, `foodItemId`, `rating`, `comment`, `date`) VALUES
(1, 1, 5, 'Excellent taste and texture!', '2024-06-15 08:30:00'),
(2, 2, 4, 'Crispy and delicious, could use a bit more filling.', '2024-06-15 09:00:00'),
(3, 3, 3, 'Tasty but a bit too oily.', '2024-06-15 09:30:00'),
(1, 4, 4, 'Light and flavorful.', '2024-06-15 10:00:00'),
(1, 5, 5, 'Perfectly cooked and very tasty.', '2024-06-15 10:30:00'),
(2, 6, 4, 'Rich and creamy, but a bit too spicy.', '2024-06-15 11:00:00'),
(2, 7, 5, 'Absolutely loved the spices!', '2024-06-15 11:30:00'),
(2, 8, 3, 'Good, but the vegetables could be fresher.', '2024-06-15 12:00:00'),
(2, 9, 4, 'Simple and comforting.', '2024-06-15 12:30:00'),
(2, 10, 5, 'Perfect balance of flavors.', '2024-06-15 13:00:00'),
(2, 11, 4, 'Creamy and delicious.', '2024-06-15 13:30:00'),
(3, 12, 5, 'Crispy and spicy, just right.', '2024-06-15 14:00:00'),
(3, 13, 3, 'Too much potato, not enough spices.', '2024-06-15 14:30:00'),
(1, 14, 4, 'Crunchy and flavorful.', '2024-06-15 15:00:00'),
(2, 15, 5, 'Perfectly cooked and seasoned.', '2024-06-15 15:30:00'),
(3, 16, 4, 'Tender and full of flavor.', '2024-06-15 16:00:00'),
(1, 17, 5, 'The spices are just right, very tasty.', '2024-06-15 16:30:00'),
(2, 18, 3, 'Too greasy for my taste.', '2024-06-15 17:00:00'),
(3, 19, 4, 'Great texture and flavor.', '2024-06-15 17:30:00'),
(1, 20, 5, 'Perfect sweetness, loved it!', '2024-06-15 18:00:00');
