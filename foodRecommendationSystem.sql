CREATE DATABASE IF NOT EXISTS FoodRecommendationSystem;

USE FoodRecommendationSystem;

CREATE TABLE `Category`(
    `categoryId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `categoryName` VARCHAR(255) NOT NULL
);

INSERT INTO `Category`(`categoryName`) VALUES('Breakfast'), ('Lunch'), ('Dinner');

CREATE TABLE `FoodItem`(
    `foodItemId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `itemName` VARCHAR(255) NOT NULL,
    `price` DOUBLE NOT NULL,
    `availabilityStatus` TINYINT(1) NOT NULL DEFAULT '1',
    `categoryId` BIGINT UNSIGNED NOT NULL REFERENCES `Category`(`categoryId`) ON DELETE CASCADE
);

ALTER TABLE FoodItem ADD COLUMN isDiscarded TINYINT(1) NOT NULL DEFAULT 0;

CREATE TABLE `Attribute`(
    `attributeId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `attributeName` VARCHAR(255) NOT NULL
);

INSERT INTO `Attribute`(`attributeName`) VALUES('Spicy'), ('Sweet'), ('Sour'), ('Salty'), ('Bitter'), ('Vegetarian'), ('Non-Vegetarian'), ('Eggitarian'), ('Vegan');

CREATE TABLE `FoodItemAttribute`(
    `foodItemId` BIGINT UNSIGNED NOT NULL,
    `attributeId` BIGINT UNSIGNED NOT NULL,
    PRIMARY KEY (`foodItemId`, `attributeId`)
);

ALTER TABLE `FoodItemAttribute`
ADD CONSTRAINT `fk_foodItemId` FOREIGN KEY (`foodItemId`) REFERENCES `FoodItem`(`foodItemId`) ON DELETE CASCADE,
ADD CONSTRAINT `fk_foodAttributeId` FOREIGN KEY (`attributeId`) REFERENCES `Attribute`(`attributeId`) ON DELETE CASCADE;


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

CREATE TABLE `ActivityType`(
    `activityTypeId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `activityType` VARCHAR(255) NOT NULL
);

INSERT INTO `ActivityType`(`activityType`) VALUES('Login'), ('Logout');

CREATE TABLE `UserActivity`(
    `activityId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `userId` BIGINT UNSIGNED NOT NULL,
    `activityTime` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    `activityTypeId` BIGINT UNSIGNED NOT NULL,
    FOREIGN KEY (`activityTypeId`) REFERENCES `ActivityType`(`activityTypeId`) ON DELETE CASCADE,
    FOREIGN KEY (`userId`) REFERENCES `User`(`userId`) ON DELETE CASCADE
);

CREATE TABLE `UserFoodPreference` (
    `userId` BIGINT UNSIGNED NOT NULL REFERENCES `User`(`userId`) ON DELETE CASCADE,
    `attributeId` BIGINT UNSIGNED NOT NULL REFERENCES `Attribute`(`attributeId`) ON DELETE CASCADE,
    PRIMARY KEY (`userId`, `attributeId`)
);

ALTER TABLE `UserFoodPreference`
ADD CONSTRAINT `fk_userId` FOREIGN KEY (`userId`) REFERENCES `User`(`userId`) ON DELETE CASCADE,
ADD CONSTRAINT `fk_attributeId` FOREIGN KEY (`attributeId`) REFERENCES `Attribute`(`attributeId`) ON DELETE CASCADE;

CREATE TABLE `Notification`(
    `notificationId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `message` TEXT NOT NULL,
    `date` DATE NOT NULL DEFAULT (CURRENT_DATE)
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


CREATE TABLE `Menu`(
    `menuId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `menuName` VARCHAR(255) NOT NULL,
    `categoryId` BIGINT UNSIGNED NULL REFERENCES `Category`(`categoryId`) ON DELETE SET NULL,
    `date` DATE NOT NULL DEFAULT (CURRENT_DATE),
    `isSurvey` BOOLEAN NOT NULL DEFAULT 0
);


CREATE TABLE `Review`(
    `reviewId` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `userId` BIGINT UNSIGNED NULL REFERENCES `User`(`userId`) ON DELETE SET NULL,
    `foodItemId` BIGINT UNSIGNED NOT NULL REFERENCES `FoodItem`(`foodItemId`) ON DELETE CASCADE,
    `rating` INT NOT NULL,
    `comment` TEXT NOT NULL,
    `date` DATE NOT NULL DEFAULT (CURRENT_DATE)
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
    `date` DATE NOT NULL DEFAULT (CURRENT_DATE),
    `preference` BOOLEAN NOT NULL
);

ALTER TABLE Feedback
ADD CONSTRAINT unique_feedback UNIQUE (userId, foodItemId, date);

CREATE TABLE DiscardFeedbackQuestion (
    questionId BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    foodItemId BIGINT UNSIGNED NOT NULL,
    question TEXT,
    FOREIGN KEY (foodItemId) REFERENCES FoodItem(foodItemId)
);

CREATE TABLE DiscardFeedbackAnswer (
    answerId BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    questionId BIGINT UNSIGNED NOT NULL,
    userId BIGINT UNSIGNED NOT NULL,
    answer TEXT,
    FOREIGN KEY (questionId) REFERENCES DiscardFeedbackQuestion(questionId) ON DELETE CASCADE,
    FOREIGN KEY (userId) REFERENCES User(userId) ON DELETE CASCADE,
    UNIQUE (questionId, userId)
);


DELIMITER $$

CREATE TRIGGER before_insert_discardFeedbackQuestion
BEFORE INSERT ON DiscardFeedbackQuestion
FOR EACH ROW
BEGIN
    DECLARE is_discarded BOOLEAN;
    
    SELECT isDiscarded INTO is_discarded 
    FROM FoodItem 
    WHERE foodItemId = NEW.foodItemId;
    
    IF is_discarded = 0 THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Cannot insert into DiscardFeedbackQuestion: the food item is not discarded.';
    END IF;
END $$

CREATE TRIGGER after_update_foodItem
AFTER UPDATE ON FoodItem
FOR EACH ROW
BEGIN
    IF NEW.isDiscarded = 0 AND OLD.isDiscarded = 1 THEN
        DELETE FROM DiscardFeedbackQuestion WHERE foodItemId = NEW.foodItemId;
    END IF;
END $$

CREATE TRIGGER trg_add_user_to_last_notification
AFTER INSERT ON User
FOR EACH ROW
BEGIN
    INSERT INTO UserLastNotification (userId, lastNotificationId)
    VALUES (NEW.userId, NULL);
END $$

CREATE TRIGGER trg_remove_null_feedback
AFTER UPDATE ON Review
FOR EACH ROW
BEGIN
    IF NEW.userId IS NULL AND NEW.foodItemId IS NULL THEN
        DELETE FROM Review WHERE userId IS NULL AND foodItemId IS NULL;
    END IF;
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

CREATE TRIGGER trg_create_menu_notification
AFTER INSERT ON Menu
FOR EACH ROW
BEGIN
    DECLARE notification_message TEXT;
    
    IF NEW.isSurvey THEN
        SET notification_message = CONCAT('Next Menu Rolled out for Survey, name: "', NEW.menuName, '"');
    ELSE
        SET notification_message = CONCAT('New menu "', NEW.menuName, '" added');
    END IF;
    
    IF NEW.categoryId IS NOT NULL THEN
        SET notification_message = CONCAT(notification_message, ' in category "', (SELECT categoryName FROM Category WHERE categoryId = NEW.categoryId), '"');
    END IF;
    
    SET notification_message = CONCAT(notification_message, ' on ', NEW.date);
    
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


DELIMITER ;	

INSERT INTO `FoodItem`(`itemName`, `price`, `availabilityStatus`, `categoryId`) VALUES
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
(1, 1, 5, 'Excellent taste and texture!', '2024-06-15'),
(2, 2, 4, 'Crispy and delicious, could use a bit more filling.', '2024-06-15'),
(3, 3, 3, 'Tasty but a bit too oily.', '2024-06-15'),
(1, 4, 4, 'Light and flavorful.', '2024-06-15'),
(1, 5, 5, 'Perfectly cooked and very tasty.', '2024-06-15'),
(2, 6, 4, 'Rich and creamy, but a bit too spicy.', '2024-06-15'),
(2, 7, 5, 'Absolutely loved the spices!', '2024-06-15 11:30:00'),
(2, 8, 3, 'Good, but the vegetables could be fresher.', '2024-06-15'),
(2, 9, 4, 'Simple and comforting.', '2024-06-15'),
(2, 10, 5, 'Perfect balance of flavors.', '2024-06-15'),
(2, 11, 4, 'Creamy and delicious.', '2024-06-15'),
(3, 12, 5, 'Crispy and spicy, just right.', '2024-06-15'),
(3, 13, 3, 'Too much potato, not enough spices.', '2024-06-15'),
(1, 14, 4, 'Crunchy and flavorful.', '2024-06-15'),
(2, 15, 5, 'Perfectly cooked and seasoned.', '2024-06-15'),
(3, 16, 4, 'Tender and full of flavor.', '2024-06-15'),
(1, 17, 5, 'The spices are just right, very tasty.', '2024-06-15'),
(2, 18, 3, 'Too greasy for my taste.', '2024-06-15'),
(3, 19, 4, 'Great texture and flavor.', '2024-06-15'),
(1, 20, 5, 'Perfect sweetness, loved it!', '2024-06-15');


INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(1, 6);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(2, 6);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(3, 6);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(4, 6);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(5, 6);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(6, 6);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(7, 7);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(8, 6);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(9, 6);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(10, 6);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(11, 6);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(12, 6);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(13, 6);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(14, 6);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(15, 7);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(16, 7);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(17, 6);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(18, 6);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(19, 7);
INSERT INTO `FoodItemAttribute`(`foodItemId`, `attributeId`) VALUES
(20, 2);