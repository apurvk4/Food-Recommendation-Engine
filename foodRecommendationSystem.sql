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
    `roleId` BIGINT UNSIGNED NOT NULL REFERENCES `Role`(`roleId`) ON DELETE CASCADE
);

INSERT INTO `User`(`name`, `password`, `roleId`) VALUES('admin', 'admin', 1), ('employee', 'employee', 2), ('chef', 'chef', 3);

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

CREATE TABLE `ScheduledMenu`(
    `scheduledMenuId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `menuId` BIGINT UNSIGNED NOT NULL REFERENCES `Menu`(`menuId`) ON DELETE CASCADE,
    `date` DATETIME NOT NULL
);

ALTER TABLE
    `ScheduledMenu` ADD INDEX `scheduledmenu_date_index`(`date`);

CREATE TABLE `Review`(
    `reviewId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `menuId` BIGINT UNSIGNED NULL REFERENCES `Menu`(`menuId`) ON DELETE SET NULL,
    `userId` BIGINT UNSIGNED NULL REFERENCES `User`(`userId`) ON DELETE SET NULL,
    `foodItemId` BIGINT UNSIGNED NOT NULL REFERENCES `FoodItem`(`foodItemId`) ON DELETE CASCADE,
    `rating` INT NOT NULL,
    `comment` TEXT NOT NULL,
    `date` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
);

ALTER TABLE
    `Review` ADD INDEX `reviews_menuid_index`(`menuId`);

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

CREATE TABLE `Session` (
    `sessionId` VARCHAR(255) PRIMARY KEY,
    `userId` BIGINT UNSIGNED NOT NULL REFERENCES `User`(`userId`) ON DELETE CASCADE,
    `createdAt` DATETIME DEFAULT CURRENT_TIMESTAMP,
    `validTill` DATETIME NOT NULL
);

CREATE TABLE `UserLastNotification` (
    `userId` BIGINT UNSIGNED NOT NULL,
    `lastNotificationId` BIGINT UNSIGNED NULL,
    PRIMARY KEY (`userId`),
    FOREIGN KEY (`userId`) REFERENCES `User`(`userId`) ON DELETE CASCADE,
    FOREIGN KEY (`lastNotificationId`) REFERENCES `Notification`(`notificationId`)
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
    IF NEW.userId IS NULL AND NEW.menuId IS NULL THEN
        DELETE FROM Review WHERE userId IS NULL AND menuId IS NULL;
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